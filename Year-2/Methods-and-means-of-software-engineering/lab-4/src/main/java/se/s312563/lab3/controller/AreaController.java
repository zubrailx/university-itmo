package se.s312563.lab3.controller;

import se.s312563.lab3.mbean.*;
import se.s312563.lab3.mbean.area.AreaBean;
import se.s312563.lab3.view.FormView;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.ViewScoped;
import javax.faces.context.FacesContext;
import javax.servlet.http.HttpSession;

@ManagedBean(name = "areaController")
@ViewScoped
public class AreaController {

    @ManagedProperty("#{formView}")
    private FormView formView;

    private AreaBean areaBean;


    @PostConstruct
    public void init() {
        areaBean = new AreaBean(this);
        HttpSession session = (HttpSession) FacesContext.getCurrentInstance().getExternalContext().getSession(false);
        if (session != null) {
            MBeanSessionLocalAgent.registerMBean(areaBean, session.getId());
        }
    }

    @PreDestroy
    public void preDestroy() {
        HttpSession session = (HttpSession) FacesContext.getCurrentInstance().getExternalContext().getSession(false);
        if (session != null) {
            MBeanSessionLocalAgent.unregisterMBean(areaBean, session.getId());
        }
    }

    public FormView getFormView() {
        return formView;
    }

    public void setFormView(FormView formView) {
        this.formView = formView;
    }

    public double calculateArea() {
        double r =  formView.getInputR();
        double area;
        area =  r * r + Math.PI * (r * r) * 1 / 4 + r * r * 1 / 4;
        return area;
    }
}
