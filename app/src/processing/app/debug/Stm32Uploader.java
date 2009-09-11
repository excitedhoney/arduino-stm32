/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Stm32Uploader - uploader implementation using stmdude
  Part of the Arduino project - http://www.arduino.cc/

  Copyright (c) 2004-05
  Hernando Barragan

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
  
  $Id: Stm32Uploader.java 609 2009-06-01 19:27:21Z dmellis $
*/

package processing.app.debug;

import processing.app.Base;
import processing.app.Preferences;
import processing.app.Serial;

import java.io.*;
import java.util.*;
import java.util.zip.*;
import javax.swing.*;
import gnu.io.*;


public class Stm32Uploader extends Uploader  {
  public Stm32Uploader() {
  }

  // XXX: add support for uploading sketches using a programmer
  public boolean uploadUsingPreferences(String buildPath, String className)
  throws RunnerException {
    String uploadUsing = Preferences.get("boards." + Preferences.get("board") + ".upload.using");
    if (uploadUsing == null) {
      // fall back on global preference
      uploadUsing = Preferences.get("upload.using");
    }
    if (uploadUsing.equals("bootloader")) {
      return uploadViaBootloader(buildPath, className);
    } else {
    /* Only serial bootloader is currently supported */
    /*
      Collection params = getProgrammerCommands(uploadUsing);
      params.add("-Uflash:w:" + buildPath + File.separator + className + ".hex:i");
      return stm32dude(params);
    */
    return false;
    }
  }
  
  private boolean uploadViaBootloader(String buildPath, String className)
  throws RunnerException {
    List commandDownloader = new ArrayList();
    commandDownloader.add("-P" + (Base.isWindows() ? "\\\\.\\" : "") + Preferences.get("serial.port"));
    commandDownloader.add(
      "-b" + Preferences.getInteger("boards." + Preferences.get("board") + ".upload.speed"));
    commandDownloader.add("-e"); // erase
    commandDownloader.add("-Uflash:w:" + buildPath + File.separator + className + ".hex:i");

    if (Preferences.get("boards." + Preferences.get("board") + ".upload.disable_flushing") == null ||
        Preferences.getBoolean("boards." + Preferences.get("board") + ".upload.disable_flushing") == false) {
      flushSerialBuffer();
    }

    return stm32dude(commandDownloader);
  }
  
  public boolean burnBootloader(String programmer) throws RunnerException {
    return burnBootloader(getProgrammerCommands(programmer));
  }
  
  private Collection getProgrammerCommands(String programmer) {
    List params = new ArrayList();
    params.add("-c" + Preferences.get("programmers." + programmer + ".protocol"));
    
    if ("usb".equals(Preferences.get("programmers." + programmer + ".communication"))) {
      params.add("-Pusb");
    } else if ("serial".equals(Preferences.get("programmers." + programmer + ".communication"))) {
      params.add("-P" + (Base.isWindows() ? "\\\\.\\" : "") + Preferences.get("serial.port"));
      // XXX: add support for specifying the baud rate for serial programmers.
    }
    // XXX: add support for specifying the port address for parallel
    // programmers, although avrdude has a default that works in most cases.
    
    if (Preferences.get("programmers." + programmer + ".force") != null &&
        Preferences.getBoolean("programmers." + programmer + ".force"))
      params.add("-F");
    
    if (Preferences.get("programmers." + programmer + ".delay") != null)
      params.add("-i" + Preferences.get("programmers." + programmer + ".delay"));
    
    return params;
  }
  
  protected boolean burnBootloader(Collection params)
  throws RunnerException {
  /* We cannot burn bootloaders to STM32 chips */

    return false;
  }
  
  public boolean stm32dude(Collection p1, Collection p2) throws RunnerException {
    ArrayList p = new ArrayList(p1);
    p.addAll(p2);
    return stm32dude(p);
  }
  
  public boolean stm32dude(Collection params) throws RunnerException {
    List commandDownloader = new ArrayList();
    commandDownloader.add("stmdude");

    // Point stmdude at its config file since it's in a non-standard location.
    /* stmdude has no configuration file */

    if (Preferences.getBoolean("upload.verbose")) {
      commandDownloader.add("-v");
      commandDownloader.add("-v");
      commandDownloader.add("-v");
      commandDownloader.add("-v");
    } else {
      commandDownloader.add("-q");
      commandDownloader.add("-q");
    }

    commandDownloader.addAll(params);

    return executeUploadCommand(commandDownloader);
  }
}
