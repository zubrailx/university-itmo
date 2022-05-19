package se.s312563.lab3.view;

import lombok.Getter;
import lombok.Setter;
import se.s312563.lab3.dto.PointDTO;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

@ManagedBean(name = "formView", eager = true)
@SessionScoped()
@Getter
@Setter
public class FormView implements Serializable {

    private final int[] valueX = {-3, -2, -1, 0, 1, 2, 3};
    private final boolean[] booleanX = new boolean[7];

    private double inputY;
    private double inputR;

    // svg
    private double svgX;
    private double svgY;

    // table dto list
    private List<PointDTO> dtoList = new ArrayList<>();

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

    public void clearForm() {
        for (int i = 0; i < booleanX.length; ++i) {
            this.booleanX[i] = false;
        }
        inputY = 0D;
        inputR = 1D;
    }
}
