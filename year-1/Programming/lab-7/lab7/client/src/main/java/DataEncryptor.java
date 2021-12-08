import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class DataEncryptor {

    public String encryptStringSHA256(String str) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            byte[] encodedBytes = md.digest(str.getBytes(StandardCharsets.UTF_8));
            StringBuilder encodedSB = new StringBuilder(new BigInteger(1, encodedBytes).toString());
            encodedSB.append("=3d2ga");
            encodedBytes = md.digest(encodedSB.toString().getBytes(StandardCharsets.UTF_8));
            encodedSB = new StringBuilder(new BigInteger(1, encodedBytes).toString(16));

            while (encodedSB.length() < 32) {
                encodedSB.insert(0, "0");
            }
            return encodedSB.toString();
        } catch (NoSuchAlgorithmException ex) {
            System.err.println("Нет заявленного алгоритма шифрования");
        }
        return null;
    }
}
