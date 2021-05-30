package file;

import java.io.*;

/**
 * Класс, отвечающий за чтение/запись данных из файла/в файл.
 */
public class FileManager {
    /**
     * Метод, производящий чтение данных из указанного файла. В случае критических ошибок программа завершает свою работу.
     *
     * @param filePath файл, из которого следует читать данные
     * @return строка, которая хранит все содержимое данного файла
     */
    public String readFromFile(String filePath) {
        FileInputStream fileInputStream = null;
        InputStreamReader inputStreamReader = null;

        StringBuilder sb = new StringBuilder();

        try {
            fileInputStream = new FileInputStream(filePath);
            inputStreamReader = new InputStreamReader(fileInputStream);

            while (inputStreamReader.ready()) {
                sb.append((char) inputStreamReader.read());
            }
        } catch (IOException ex) {
            System.err.println("Произошла ошибка при добавлении файла во входящий поток " + ex);
            System.exit(-1);
        } catch (NullPointerException ex) {
            System.err.println("Не указан файл, из которого следует читать данные " + ex);
            System.exit(-1);
        } finally {
            try {
                if (fileInputStream != null) {
                    fileInputStream.close();
                }
                if (inputStreamReader != null) {
                    inputStreamReader.close();
                }
            } catch (IOException ex) {
                System.err.println("Произошла ошибка при закрытии " + ex);
            }
        }
        return sb.toString();
    }

    /**
     * Метод, производящий запись данных в указанный файл.
     *
     * @param filePath файл, куда следует записывать данные
     * @param str      строка, которую следует записать в файл
     */
    public void writeToFile(String str, String filePath) {
        FileOutputStream fileOutputStream = null;
        PrintWriter printWriter = null;

        try {
            fileOutputStream = new FileOutputStream(filePath);
            printWriter = new PrintWriter(fileOutputStream);

            printWriter.write(str);


        } catch (IOException ex) {
            System.err.println("Произошла ошибка при добавлении файла в исходнящий поток\n" + ex);
        } catch (NullPointerException ex) {
            System.err.println("Не указан файл, куда следует записывать данные " + ex);
        } finally {
            try {
                if (printWriter != null) {
                    printWriter.close();
                }
                if (fileOutputStream != null) {
                    fileOutputStream.close();
                }
            } catch (IOException ex) {
                System.err.println("Произошла ошибка при закрытии " + ex);
            }
        }
    }
}
