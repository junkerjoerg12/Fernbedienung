package main;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Main {
  public static void main(String[] args) {

    String text = "const char webpageCode[] = R\"=====(" + "\n";
    String line = "";

    try {
      BufferedWriter writer = new BufferedWriter(
          new FileWriter("C:\\Users\\andre\\Documents\\Arduino\\Arduino\\espwebserver\\webpageCode.h"));

      BufferedReader reader = new BufferedReader(
          new FileReader("C:\\Users\\andre\\Documents\\Arduino\\FernbedienungWebsite\\index.html"));
      while (!(line = reader.readLine()).contains("</head>")) {
        if (!line.equals("    <link rel=\"stylesheet\" href=\"style.css\" />")) {

          text += line + "\n";
        }
      }
      text += "    <style>" + "\n";
      reader.close();

      reader = new BufferedReader(
          new FileReader("C:\\Users\\andre\\Documents\\Arduino\\FernbedienungWebsite\\style.css"));
      while ((line = reader.readLine()) != null) {
        text += line + "\n";
      }
      text += "    </style>" + "\n";
      text += "  </head>" + "\n";
      reader.close();

      // hier kommt irgendwie noch der Head teilm it rein glscube ich, sonst passts
      reader = new BufferedReader(
          new FileReader("C:\\Users\\andre\\Documents\\Arduino\\FernbedienungWebsite\\index.html"));
      System.out.println("hier gehts los");
      while (!(line = reader.readLine()).contains("</head")) {
        System.out.println(line);
      }

      while (!(line = reader.readLine()).contains("<script")) {
        text += line + "\n";
      }
      text += "   <script>" + "\n";

      reader.close();

      reader = new BufferedReader(
          new FileReader("C:\\Users\\andre\\Documents\\Arduino\\FernbedienungWebsite\\index.js"));

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
