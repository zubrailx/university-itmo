package se.s312563.lab3.mbean;

import javax.management.InstanceAlreadyExistsException;
import javax.management.MBeanServer;
import javax.management.MalformedObjectNameException;
import javax.management.ObjectName;
import java.lang.management.ManagementFactory;

public class MBeanSessionLocalAgent {
    private static final MBeanServer mbs;

    static {
        mbs = ManagementFactory.getPlatformMBeanServer();
    }

    public static void registerMBean(Object object, String name) {
        // want to create a class
        try {
            ObjectName objectName = getObjectName(object, name);
            mbs.registerMBean(object, objectName);
            System.out.println("Register: " + objectName.getCanonicalName());
        } catch (InstanceAlreadyExistsException ex) {
            System.out.println("Instance already exists: " + ex.getMessage());
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
    public static void unregisterMBean(Object object, String name) {
        try {
            ObjectName objectName = getObjectName(object, name);
            mbs.unregisterMBean(objectName);
            System.out.println("Unregister: " + objectName.getCanonicalName());
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private static ObjectName getObjectName(Object object, String name) throws MalformedObjectNameException {
        String pckg = object.getClass().getPackage().getName();
        String clss = object.getClass().getSimpleName();
        String objname = pckg + ":type=" + clss + ",name=" + name;
        return new ObjectName(objname);
    }
}