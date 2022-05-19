package se.s312563.lab3.mbean.area;

import se.s312563.lab3.controller.AreaController;


public class AreaBean implements AreaBeanMBean {

    private final AreaController areaController;

    public AreaBean(AreaController areaController) {
        this.areaController = areaController;
    }

    @Override
    public double getRadius() {
        return areaController.getFormView().getInputR();
    }

    @Override
    public void setRadius(double r) {
        areaController.getFormView().setInputR(r);
    }

    @Override
    public double calculateArea() {
        return areaController.calculateArea();
    }
}
