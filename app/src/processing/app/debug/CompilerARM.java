/* -*- mode: java; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Processing project - http://processing.org

  Copyright (c) 2004-08 Ben Fry and Casey Reas
  Copyright (c) 2001-04 Massachusetts Institute of Technology

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

package processing.app.debug;

import processing.app.Base;
import processing.app.Preferences;
import processing.app.Sketch;
import processing.app.SketchCode;
import processing.core.*;

import java.io.*;
import java.util.*;
import java.util.zip.*;


public class CompilerARM extends Compiler {
  static final String BUGS_URL =
    "https://developer.berlios.de/bugs/?group_id=3590";
  static final String SUPER_BADNESS =
    "Compiler error, please submit this code to " + BUGS_URL;
  static String armToolChain = "arm-none-eabi-";

  public CompilerARM() { }

  /**
   * Compile with arm-none-eabi-gcc.
   *
   * @param sketch Sketch object to be compiled.
   * @param buildPath Where the temporary files live and will be built from.
   * @param primaryClassName the name of the combined sketch file w/ extension
   * @param target the target (core) to build against
   * @return true if successful.
   * @throws RunnerException Only if there's a problem. Only then.
   */
  public boolean compile(Sketch sketch,
                         String buildPath,
                         String primaryClassName,
			 Target target) throws RunnerException {
    this.sketch = sketch;
    this.buildPath = buildPath;
    this.primaryClassName = primaryClassName;

    // the pms object isn't used for anything but storage
    MessageStream pms = new MessageStream(this);

    String userdir = System.getProperty("user.dir") + File.separator;
    String toolchainBasePath = Base.getAvrBasePath();
    
    List<File> objectFiles = new ArrayList<File>();

    List includePaths = new ArrayList();
    includePaths.add(target.getPath());
    
    String runtimeLibraryName = buildPath + File.separator + "core.a";

    // 1. compile the target (core), outputting .o files to <buildPath> and
    // then collecting them into the core.a library file.
    
    List<File> targetObjectFiles = 
      compileFiles(toolchainBasePath, buildPath, includePaths,
                   findFilesInPath(target.getPath(), "c", true),
                   findFilesInPath(target.getPath(), "cpp", true));
                   
    List baseCommandAR = new ArrayList(Arrays.asList(new String[] {
      toolchainBasePath + armToolChain+"ar",
      "rcs",
      runtimeLibraryName
    }));

    for(File file : targetObjectFiles) {
      List commandAR = new ArrayList(baseCommandAR);
      commandAR.add(file.getAbsolutePath());
      execAsynchronously(commandAR);
    }

    // 2. compile the libraries, outputting .o files to: <buildPath>/<library>/
    
    // use library directories as include paths for all libraries
    for (File file : sketch.getImportedLibraries()) {
      includePaths.add(file.getPath());
    }
    
    for (File libraryFolder : sketch.getImportedLibraries()) {
      File outputFolder = new File(buildPath, libraryFolder.getName());
      createFolder(outputFolder);
      // this library can use includes in its utility/ folder
      includePaths.add(libraryFolder.getPath() + File.separator + "utility");
      objectFiles.addAll(
        compileFiles(toolchainBasePath, outputFolder.getAbsolutePath(), includePaths,
                     findFilesInFolder(libraryFolder, "c", false),
                     findFilesInFolder(libraryFolder, "cpp", false)));
      outputFolder = new File(outputFolder, "utility");
      createFolder(outputFolder);
      objectFiles.addAll(
        compileFiles(toolchainBasePath, outputFolder.getAbsolutePath(), includePaths,
                     findFilesInFolder(new File(libraryFolder, "utility"), "c", false),
                     findFilesInFolder(new File(libraryFolder, "utility"), "cpp", false)));
      // other libraries should not see this library's utility/ folder
      includePaths.remove(includePaths.size() - 1);
    }
    
    // 3. compile the sketch (already in the buildPath)
    
    objectFiles.addAll(
      compileFiles(toolchainBasePath, buildPath, includePaths,
                   findFilesInPath(buildPath, "c", false),
                   findFilesInPath(buildPath, "cpp", false)));
                   
    // 4. link it all together into the .elf file
    
    List baseCommandLinker = new ArrayList(Arrays.asList(new String[] {
      toolchainBasePath + armToolChain+"gcc",
      "-Os",
      "-mcpu=" + Preferences.get("boards." + Preferences.get("board") + ".build.mcu"),
      "-mthumb", // Cortex-M3 only supports thumb mode
      "-nostartfiles",
      "-Wl,-gc-sections,--script=" + target.getPath() + File.separator + "stm32f103_rom.cmd",
      "-Wl,-Map,"+
      buildPath + File.separator + primaryClassName + ".elf.map",
      "-o",
      buildPath + File.separator + primaryClassName + ".elf"
    }));
    
    for (File file : objectFiles) {
      baseCommandLinker.add(file.getAbsolutePath());
    }

    baseCommandLinker.add(buildPath + File.separator + "vectors_stm32.c.o");
    baseCommandLinker.add(buildPath + File.separator + "syscalls.c.o");
    baseCommandLinker.add(runtimeLibraryName);
    baseCommandLinker.add("-L" + buildPath);
    baseCommandLinker.add("-lm");

    execAsynchronously(baseCommandLinker);

    List baseCommandObjcopy = new ArrayList(Arrays.asList(new String[] {
      toolchainBasePath + armToolChain+"objcopy",
      "-O",
    }));
    
    List commandObjcopy;

    // 6. build the .hex file
    commandObjcopy = new ArrayList(baseCommandObjcopy);
    commandObjcopy.add("ihex");
    commandObjcopy.add(buildPath + File.separator + primaryClassName + ".elf");
    commandObjcopy.add(buildPath + File.separator + primaryClassName + ".hex");
    execAsynchronously(commandObjcopy);
    
    return true;
  }
  
  protected List<File> compileFiles(String avrBasePath,
                                  String buildPath, List<File> includePaths,
                                  List<File> cSources, List<File> cppSources)
    throws RunnerException {
    
    List<File> objectPaths = new ArrayList<File>();
    
    for (File file : cSources) {
        String objectPath = buildPath + File.separator + file.getName() + ".o";
        objectPaths.add(new File(objectPath));
        execAsynchronously(getCommandCompilerC(avrBasePath, includePaths,
                                               file.getAbsolutePath(),
                                               objectPath));
    }
    
    for (File file : cppSources) {
        String objectPath = buildPath + File.separator + file.getName() + ".o";
        objectPaths.add(new File(objectPath));
        execAsynchronously(getCommandCompilerCPP(avrBasePath, includePaths,
                                                 file.getAbsolutePath(),
                                                 objectPath));
    }
    
    return objectPaths;
  }
    
  
  /////////////////////////////////////////////////////////////////////////////

  static public List getCommandCompilerC(String toolchainBasePath, List includePaths,
    String sourceName, String objectName) {
    List baseCommandCompiler = new ArrayList(Arrays.asList(new String[] {
      toolchainBasePath + armToolChain+"gcc",
      "-c", // compile, don't link
      "-g", // include debugging info (so errors include line numbers)
      "-Os", // optimize for size
      "-w", // surpress all warnings
      "-ffunction-sections", // place each function in its own section
      "-fdata-sections",
      "-mcpu=" + Preferences.get("boards." + Preferences.get("board") + ".build.mcu"),
      "-mthumb", // Cortex-M3 only supports thumb mode
      "-DF_CPU=" + Preferences.get("boards." + Preferences.get("board") + ".build.f_cpu"),
      "-DSTACKTOP=" + Preferences.get("boards." + Preferences.get("board") + ".build.stacktop"),
    }));

    for (int i = 0; i < includePaths.size(); i++) {
      baseCommandCompiler.add("-I" + (String) includePaths.get(i));
    }
    
    baseCommandCompiler.add("-Wa,-ahls=" + objectName + ".lst");
    baseCommandCompiler.add(sourceName);
    baseCommandCompiler.add("-o"+ objectName);
    
    return baseCommandCompiler;
  }
  
  
  static public List getCommandCompilerCPP(String toolchainBasePath,
    List includePaths, String sourceName, String objectName) {
      List baseCommandCompilerCPP = new ArrayList(Arrays.asList(new String[] {
      toolchainBasePath + armToolChain+"g++",
      "-c", // compile, don't link
      "-g", // include debugging info (so errors include line numbers)
      "-Os", // optimize for size
      "-w", // surpress all warnings
      "-fno-rtti",
      "-fno-exceptions",
      "-ffunction-sections", // place each function in its own section
      "-fdata-sections",
      "-mcpu=" + Preferences.get("boards." + Preferences.get("board") + ".build.mcu"),
      "-mthumb", // Cortex-M3 only supports thumb mode
      "-DF_CPU=" + Preferences.get("boards." + Preferences.get("board") + ".build.f_cpu"),
    }));

    for (int i = 0; i < includePaths.size(); i++) {
      baseCommandCompilerCPP.add("-I" + (String) includePaths.get(i));
    }
    
    baseCommandCompilerCPP.add("-Wa,-ahls=" + objectName + ".lst");
    baseCommandCompilerCPP.add(sourceName);
    baseCommandCompilerCPP.add("-o"+ objectName);
    
    return baseCommandCompilerCPP;
  }

  
  static public List getCommandLinker(String toolchainBasePath,
    String buildPath, Target target, Sketch sketch) {
    List baseCommandLinker = new ArrayList(Arrays.asList(new String[] {
      toolchainBasePath + armToolChain+"gcc",
      "-Os",
      "-mcpu=" + Preferences.get("boards." + Preferences.get("board") + ".build.mcu"),
      "-mthumb", // Cortex-M3 only supports thumb mode
      "-nostartfiles",
      "-Wl,-gc-sections,--script=" + target.getPath() + File.separator + "stm32f103_rom.cmd",
      "-Wl,-Map,"+
      ((!Base.isMacOS()) ? buildPath : buildPath) + File.separator + sketch.getName() + ".elf.map",
      "-o",
      ((!Base.isMacOS()) ? buildPath : buildPath) + File.separator + sketch.getName() + ".elf"
    }));

    return baseCommandLinker;
  }

}
