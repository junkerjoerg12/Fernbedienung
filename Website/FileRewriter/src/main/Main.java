package main;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Main {
  public static void main(String[] args) {

    // liest html, css und js Datein ein und schreibt sie in einen .h datei um, die
    // von Arduino einfacher gelesen werden kann

    String text = "const char webpageCode[] = R\"=====(" + "\n";
    String line = "";

    try {
      File file = new File("Website/webpagecode.h");

      if (!file.exists()) {
        file.createNewFile();
      }
      BufferedWriter writer = new BufferedWriter(new FileWriter(file));

      BufferedReader reader = new BufferedReader(
          new FileReader("Website/index.html"));
      while (!(line = reader.readLine()).contains("</head>")) {
        if (!line.equals("    <link rel=\"stylesheet\" href=\"style.css\" />")) {

          text += line + "\n";
        }
      }
      text += "    <style>" + "\n";
      reader.close();

      reader = new BufferedReader(
          new FileReader("Website/style.css"));
      while ((line = reader.readLine()) != null) {
        text += line + "\n";
      }
      text += "    </style>" + "\n";
      text += "  </head>" + "\n";
      reader.close();

      reader = new BufferedReader(
          new FileReader("Website/index.html"));
      while (!(line = reader.readLine()).contains("</head")) {
      }

      while (!(line = reader.readLine()).contains("<script")) {
        text += line + "\n";
      }
      text += "   <script>" + "\n";

      reader.close();

      reader = new BufferedReader(
          new FileReader("Website/index.js"));

      while ((line = reader.readLine()) != null) {
        text += line + "\n";
      }

      text += "</script>" + "\n" + "  </body>" + "\n" + "</html>";

      text += "\n" + ")=====\";";
      writer.write(text);
      writer.close();
      reader.close();
    } catch (IOException e) {
      e.printStackTrace();
    }

  }
}
