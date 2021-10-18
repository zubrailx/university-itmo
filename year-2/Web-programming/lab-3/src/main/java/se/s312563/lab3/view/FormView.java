package se.s312563.lab3.view;

import se.s312563.lab3.dto.PointDTO;

import javax.annotation.PostConstruct;
import javax.enterprise.context.SessionScoped;
import javax.inject.Named;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

@Named("formView")
@SessionScoped
public class FormView implements Serializable {

    private final int[] valueX = {-3, -2, -1, 0, 1, 2, 3};
    private final boolean[] booleanX = new boolean[7];

    private double inputY;
    private double inputR;

    // svg
    private double svgX;
    private double svgY;

    // table dto list
    private List<PointDTO> dtoList;

    @PostConstruct
    private void init() {
        dtoList = new ArrayList<>();
    }

    public boolean[] getBooleanX() {
        return booleanX;
    }

    public int[] getValueX() {
        return valueX;
    }

    public double getInputY() {
        return inputY;
    }

    public void setInputY(double y) {
        this.inputY = y;
    }

    public double getInputR() {
        return inputR;
    }

    public void setInputR(double r) {
        this.inputR = r;
    }

    public double getSvgX() {
        return svgX;
    }

    public void setSvgX(double svgX) {
        this.svgX = svgX;
    }

    public double getSvgY() {
        return svgY;
    }

    public void setSvgY(double svgY) {
        this.svgY = svgY;
    }

    public List<PointDTO> getUserDTOList() {
        List<PointDTO> lst = new ArrayList<>();
        for (int i = 0; i < booleanX.length; ++i) {
            if (booleanX[i]) {
                lst.add(PointDTO.newBuilder()
                        .setX(valueX[i])
                        .setY(inputY)
                        .setR(inputR).build());
            }
        }
        return lst;
    }

    public PointDTO getUserDTOSvg() {
        return PointDTO.newBuilder()
                .setX(svgX)
                .setY(svgY)
                .setR(inputR).build();
    }

    public List<PointDTO> getDtoList() {
        return dtoList;
    }

    public void setDtoList(List<PointDTO> dtoList) {
        this.dtoList = dtoList;
    }

    public void clearForm() {
        for (int i = 0; i < booleanX.length; ++i) {
            this.booleanX[i] = false;
        }
        inputY = 0D;
        inputR = 1D;
    }
}
