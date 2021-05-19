package run;

public class Main {
    public static void main(String[] args) {

        if (args.length > 0) {
            if (!args[0].equals("")) {
                Application application = new Application();
                application.start(args[0]);
            }
        } else {
            Application application = new Application();
            application.start("C:\\Users\\Home\\Desktop\\Данные\\inputfile.txt");
        }
    }
}
