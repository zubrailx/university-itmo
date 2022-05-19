package se.s312563.lab3.controller;

import se.s312563.lab3.dto.PointDTO;
import se.s312563.lab3.mbean.MBeanSessionLocalAgent;
import se.s312563.lab3.mbean.point.PointBean;
import se.s312563.lab3.view.FormView;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.SessionScoped;
import javax.faces.context.FacesContext;
import javax.servlet.http.HttpSession;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;

@ManagedBean(name = "pointController")
@SessionScoped
public class PointController {
    private static final DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
    private static final int COUNTER_LIMIT = 4;

    @ManagedProperty(value = "#{formView}")
    private FormView formView;

    private PointBean pointBean;
    private int counter = 0;
    private String announcement = "";

    @PostConstruct
    public void init() {
        pointBean = new PointBean(this);
        HttpSession session = (HttpSession) FacesContext.getCurrentInstance().getExternalContext().getSession(false);
        if (session != null) {
            MBeanSessionLocalAgent.registerMBean(pointBean, session.getId());
        }
    }

    @PreDestroy
    public void preDestroy() {
        HttpSession session = (HttpSession) FacesContext.getCurrentInstance().getExternalContext().getSession(false);
        if (session != null) {
            MBeanSessionLocalAgent.unregisterMBean(pointBean, session.getId());
        }
    }

    public void setFormView(FormView formView) {
        this.formView = formView;
    }

    public String getAnnouncement() {
        return announcement;
    }

    public long getAllPointCount() {
        return formView.getDtoList().size();
    }

    public long getAllPointHitCount() {
        return formView.getDtoList().stream().filter(PointDTO::getHit).count();
    }


    public void updateCounter(List<PointDTO> points) {
        for (PointDTO point : points) {
            updateCounter(point);
        }
        // mbean
        pointBean.syncCounter(counter);
    }

    public void updateCounter(PointDTO pointDTO) {
        if (!pointDTO.getHit()) {
            increaseCounter(1);
            checkCounter();
        } else {
            restoreCounter();
        }
        // mbean
        pointBean.syncCounter(counter);
    }


    public void restoreCounter() {
        counter = 0;
        announcement = "";
    }


    private void increaseCounter(int value) {
        counter += value;
    }

    private void checkCounter() {
        if (counter >= COUNTER_LIMIT) {
            announcement = "[" + dtf.format(LocalDateTime.now()) + "]: " + "In a row: " + counter + " misses";
//            System.out.println(announcement);
        } else {
            announcement = "";
        }
    }
}
