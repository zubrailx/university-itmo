package se.s312563.lab3.controller;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import se.s312563.lab3.DTOConverter;
import se.s312563.lab3.dto.PointDTO;
import se.s312563.lab3.entity.Point;
import se.s312563.lab3.service.PointService;
import se.s312563.lab3.view.FormView;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.SessionScoped;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

@ManagedBean(name = "inputController")
@SessionScoped
@Getter
@Setter
public class InputController {

    @ManagedProperty(value = "#{pointService}")
    PointService pointService;

    @ManagedProperty(value = "#{formView}")
    private FormView formView;

    @ManagedProperty(value = "#{pointController}")
    private PointController pointController;


    public void executeForm() {
        List<PointDTO> dtoList = formView.getUserDTOList();
        List<Point> pointList = dtoList.stream().map(DTOConverter::dtoToEntity)
                .filter(InputValidator::validateFormEntity).collect(Collectors.toList());

        pointList = pointService.addEntityList(pointList);
        // append list
        if (pointList != null) {
            dtoList = pointList.stream().map(DTOConverter::entityToDto).collect(Collectors.toList());
            formView.setDtoList(Stream.concat(formView.getDtoList().stream(), dtoList.stream()).collect(Collectors.toList()));
            // counter
            pointController.updateCounter(dtoList);
        }
    }

    public void executeSvg() {
        PointDTO pointDTO = formView.getUserDTOSvg();
        Point p = DTOConverter.dtoToEntity(pointDTO);
        if (InputValidator.validateSvgEntity(p)) {
            p = pointService.addEntity(p);
            // append list
            if (p != null) {
                pointDTO = DTOConverter.entityToDto(p);
                formView.getDtoList().add(pointDTO);
                // counter
                pointController.updateCounter(pointDTO);
            }
        }
    }

    public void clearTable() {
        List<Point> pList = pointService.deleteSessionEntityList();
        // reduce list
        if (pList != null) {
            List<PointDTO> dtoList = pList.stream().map(DTOConverter::entityToDto).collect(Collectors.toList());
            List<PointDTO> formList = formView.getDtoList();
            formList.removeAll(dtoList);
            // counter
            pointController.restoreCounter();
        }
    }
}
