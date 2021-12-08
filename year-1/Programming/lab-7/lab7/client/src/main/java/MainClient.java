public class MainClient {

    public static void main(String[] args) {

        if (args.length != 0) {
            Application application = new Application(Integer.parseInt(args[0]));
            application.start();
        }
    }
}
