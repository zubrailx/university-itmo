package se.s312563.lab3.mbean.point;

import se.s312563.lab3.controller.PointController;

import javax.management.Notification;
import javax.management.NotificationBroadcasterSupport;


public class PointBean extends NotificationBroadcasterSupport implements PointBeanMBean {

    private final PointController pointController;

    public PointBean(PointController pointController) {
        this.pointController = pointController;
    }

    public void syncCounter(long counter) {
        if (counter >= 4) {
            Notification notification = new Notification("Count of misses in a row", this, counter, "User has already got 4 misses in a row.");
            sendNotification(notification);
        }
    }

    @Override
    public long getAllPointCount() {
        return pointController.getAllPointCount();
    }

    @Override
    public long getAllPointHitCount() {
        return pointController.getAllPointHitCount();
    }


}
