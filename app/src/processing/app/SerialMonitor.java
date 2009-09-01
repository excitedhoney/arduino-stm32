/* -*- mode: java; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
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

package processing.app;

import processing.app.debug.MessageConsumer;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.io.*;

public class SerialMonitor extends JFrame implements MessageConsumer {
  private Serial serial;
  private String port;
  private JTextArea textArea;
  private JTextField textField;
  private JButton sendButton;
  private JComboBox serialRates;
  private JLabel statusLabel;
  private int serialRate;
  private JButton logButton;
  private JButton pauseResumeButton;
  private JButton clearButton;
  private FileOutputStream logfile;
  private PrintStream log;
  final JFileChooser fc = new JFileChooser();
  private boolean paused = false;


  public SerialMonitor(String port) {
    super(port);
  
    this.port = port;
  
    addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent e) {
          closeSerialPort();
        }
      });  
      
    // obvious, no?
    KeyStroke wc = Editor.WINDOW_CLOSE_KEYSTROKE;
    getRootPane().getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).put(wc, "close");
    getRootPane().getActionMap().put("close", new AbstractAction() {
      public void actionPerformed(ActionEvent e) {
        closeSerialPort();
        setVisible(false);
      }});
  
    getContentPane().setLayout(new BorderLayout());
    
    Font font = Theme.getFont("console.font");

    textArea = new JTextArea(16, 40);
    textArea.setEditable(false);    
    textArea.setFont(font);
    
    getContentPane().add(new JScrollPane(textArea), BorderLayout.CENTER);
    
    JPanel pane = new JPanel(new BorderLayout(4, 0));
    pane.setBorder(new EmptyBorder(4, 4, 4, 4));

    textField = new JTextField(40);
    textField.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        send(textField.getText());
        textField.setText("");
      }});

    sendButton = new JButton("Send");
    sendButton.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        send(textField.getText());
        textField.setText("");
      }});
    
    pane.add(textField, BorderLayout.CENTER);
    pane.add(sendButton, BorderLayout.EAST);
    
    getContentPane().add(pane, BorderLayout.NORTH);
    
    pane = new JPanel(new BorderLayout(4, 0));
    pane.setBorder(new EmptyBorder(4, 4, 4, 4));
    
    statusLabel = new JLabel();
    
    pane.add(statusLabel, BorderLayout.CENTER);
  
    String[] serialRateStrings = {
      "300","1200","2400","4800","9600","14400",
      "19200","28800","38400","57600","115200"
    };
    
    serialRates = new JComboBox();
    for (int i = 0; i < serialRateStrings.length; i++)
      serialRates.addItem(serialRateStrings[i] + " baud");

    serialRate = Preferences.getInteger("serial.debug_rate");
    serialRates.setSelectedItem(serialRate + " baud");
    serialRates.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        String wholeString = (String) serialRates.getSelectedItem();
        String rateString = wholeString.substring(0, wholeString.indexOf(' '));
        serialRate = Integer.parseInt(rateString);
        Preferences.set("serial.debug_rate", rateString);
        closeSerialPort();
        openSerialPort();
      }});
    
    pane.add(serialRates, BorderLayout.EAST);
    
    JPanel buttonPane = new JPanel(new FlowLayout());

    /* logButton is used to direct messages to a log file */
    logButton = new JButton("Open logfile");
    logButton.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (log == null)
        {
            if (fc.showSaveDialog(getContentPane())==JFileChooser.APPROVE_OPTION)
            {
              File file = fc.getSelectedFile();
              openLogFile(file.getPath());
            }
        }
        else
        {
          closeLogFile();
        }
      }});
    buttonPane.add(logButton);
       
    /* pauseResumeButton is used to pause the message flow */
    pauseResumeButton = new JButton("Pause");
    pauseResumeButton.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        togglePause();
      }});
    buttonPane.add(pauseResumeButton);

    /* clearButton is used to clear the message window */
    clearButton = new JButton("Clear");
    clearButton.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        textArea.setText("");
      }});
    buttonPane.add(clearButton);

    pane.add(buttonPane, BorderLayout.WEST);
    getContentPane().add(pane, BorderLayout.SOUTH);

    pack();
  }
  
  private void send(String s) {
    if (serial != null)
      serial.write(s);
  }

  public void openLogFile(String filename) {
    try {
      logfile = new FileOutputStream(filename);
      log = new PrintStream( logfile );
      statusLabel.setText("opening log file: "+filename);
      logButton.setLabel("Open logfile");
    } catch (Exception e) {
      statusLabel.setText(e.getMessage());
    }
  }

  public void closeLogFile() {
    statusLabel.setText("closing log file");
    log.close();
    log = null;
    logButton.setLabel("Open logfile");
  }

  public void togglePause() {
    paused = !paused;
    if (paused)
    {
        pauseResumeButton.setLabel("Resume");
    }
    else
    {
        pauseResumeButton.setLabel("Pause");
    }
  }

  public void openSerialPort() {
    if (serial != null) return;
  
    try {
      statusLabel.setText("opening...");
      serial = new Serial(port, serialRate);
      serial.addListener(this);
      statusLabel.setText("");
    } catch (SerialException e) {
      statusLabel.setText(e.getMessage());
    }
    
    if (paused) togglePause();
  }
  
  public void closeSerialPort() {
    if (serial != null) {
      statusLabel.setText("closing...");
      textArea.append("SERIAL PORT CLOSED");
//      textArea.setText("");
      serial.dispose();
      serial = null;
      statusLabel.setText("");
    }
    if (log != null)
    {
      closeLogFile();
    }
  }
  
  public void message(final String s) {
    if (paused) return;
    SwingUtilities.invokeLater(new Runnable() {
      public void run() {
        textArea.append(s);
        if (log != null) log.print(s);
      }});
  }
}

