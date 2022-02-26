import React, { useState } from 'react';
import { useStore } from 'effector-react';

import { pointRemoveMany, pointSave } from '../../../service/apiRequests';
import Point from '../../../model/point';
import userContext from '../../../model/context';
import { addPoint, clearPointStore, $radiusStore, setRadius, $pointStore, removePointById } from '../../../state/state';

import H1 from '../../atoms/H1';
import Button from '../../atoms/Button';
import Input from '../../molecules/Input';

import "./index.scss";


const msg = {
    none: undefined,
    invalid: "Input data is invalid",
    outOfRange: "Input data is out of range",
}

const PointForm = () => {
    const [inputX, setInputX] = useState(0);
    const [inputY, setInputY] = useState(0);
    const [inputR, setInputR] = useState(useStore($radiusStore));

    const [errorX, setErrorX] = useState(msg.none);
    const [errorY, setErrorY] = useState(msg.none);
    const [errorR, setErrorR] = useState(msg.none);


    const numberRegexOnChange = /(^[+-]?\d*([.,]\d*)?$)|(^$)/
    const numberRegexOnBlur = /^[+-]?\d+([.,]\d+)?$|^$/

    const pointIds = useStore($pointStore).map(p => p.pointId);
    // input change handling
    const handleInputXChange = (e) => {
        if (e.target.value.match(numberRegexOnChange)) {
            setInputX(e.target.value);
        } else {
            e.target.value = inputX;
        }
    }

    const handleInputYChange = (e) => {
        if (e.target.value.match(numberRegexOnChange)) {
            setInputY(e.target.value);
        } else {
            e.target.value = inputY;
        }
    }

    const handleInputRChange = (e) => {
        if (e.target.value.match(numberRegexOnChange)) {
            setInputR(e.target.value);
        } else {
            e.target.value = inputR;
        }
    }

    // on blur input handling
    const handleInputXBlur = (e) => {
        if (!e.target.value.match(numberRegexOnBlur)) {
            setErrorX(msg.invalid);
        } else if (!checkRange(e.target.value, -2, 2)) {
            setErrorX(msg.outOfRange);
        } else {
            setErrorX(msg.none);
        }
    }

    const handleInputYBlur = (e) => {
        if (!e.target.value.match(numberRegexOnBlur)) {
            setErrorY(msg.invalid);
        } else if (!checkRange(e.target.value, -3, 3)) {
            setErrorY(msg.outOfRange);
        } else {
            setErrorY(msg.none);
        }
    }

    const handleInputRBlur = (e) => {
        if (!e.target.value.match(numberRegexOnBlur)) {
            setErrorR(msg.invalid);
        } else if (!checkRange(e.target.value == 0 || e.target.value, 0, 2)) {
            setErrorR(msg.outOfRange);
        } else {
            setErrorR(msg.none);
            // set radiusStore value for SVG
            setRadius(e.target.value);
        }
    }

    const checkRange = (value, min, max) => {
        const isNumeric = (n) => {
            return !isNaN(parseFloat(n)) && isFinite(n);
        }
        const isInRange = (n, min, max) => {
            return parseFloat(n) >= parseFloat(min) && parseFloat(n) <= parseFloat(max);
        }

        return isNumeric(value) && isInRange(value, min, max);
    }


    const handleFormSubmit = (e) => {
        e.preventDefault();
        if (errorX === msg.none && errorY === msg.none && errorR === msg.none) {
            let p = new Point();
            p.coordinateX = inputX;
            p.coordinateY = inputY;
            p.radius = inputR;
            pointSave(p)
                .then(res => {
                    p.pointId = res.data.pointId;
                    p.coordinateX = res.data.coordinateX;
                    p.coordinateY = res.data.coordinateY;
                    p.radius = res.data.radius;
                    p.hit = res.data.hit;
                    p.ldt = Date.parse(res.data.ldt);
                    p.stm = res.data.stm;
                    addPoint(p);
                })
                .catch(err => alert(err));
        }
        else {
            alert("bad parameters");
        }
    }

    const handleFormClean = (e) => {
        e.preventDefault();
        pointRemoveMany(pointIds)
            .then(res => res.data.forEach(p => removePointById(p.pointId)))
            .catch(err => alert(err));
    }


    return (
        <form className="form-point" >
            <div className="form-point__wrapper">
                <H1 className="form-point__header">Point Form</H1>
                <div className="form-point-x">
                    <Input error={errorX} onBlur={handleInputXBlur} onChange={handleInputXChange}
                        className="form-point-x__input" label="Coordinate X" value={inputX} />
                </div>
                <div className="form-point-y">
                    <Input error={errorY} onBlur={handleInputYBlur} onChange={handleInputYChange}
                        className="form-point-y__input" label="Coordinate Y" value={inputY} />
                </div>
                <div className="form-point-r">
                    <Input error={errorR} onBlur={handleInputRBlur} onChange={handleInputRChange}
                        className="form-point-r__input" label="Radius" value={inputR} />
                </div>
                <div className="point-buttons form-point__buttons">
                    <Button onClick={handleFormSubmit} className="point-buttons__button">Submit</Button>
                    <Button onClick={handleFormClean} className="point-buttons__button">Clean</Button>
                </div>
            </div>
        </form>
    );

}


export default PointForm;