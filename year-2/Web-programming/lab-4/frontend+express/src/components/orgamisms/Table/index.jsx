import React from 'react';
import { useStore } from 'effector-react';
import moment from 'moment';

import { $pointStore } from "../../../state/state";

import "./index.scss";


const Table = () => {
    const pointStore = useStore($pointStore);
    const tableBody = pointStore.map(e => {
        return (
            <tr>
                <td>{parseFloat(e.coordinateX).toFixed(4)}</td>
                <td>{parseFloat(e.coordinateY).toFixed(4)}</td>
                <td>{parseFloat(e.radius).toFixed(4)}</td>
                <td>{e.hit === true ? "True" : "False"}</td>
                <td>{parseFloat(e.stm).toFixed(4)}</td>
                <td>{moment(e.ldt).format("DD/MM/YYYY HH:mm:ss")}</td>
            </tr>
        );
    });

    return (
        <div className="table-container">
            <table className="table-container__table form-table">
                <thead>
                    <tr>
                        <th>Coordinate X</th>
                        <th>Coordinate Y</th>
                        <th>Radius</th>
                        <th>Fact of Hit</th>
                        <th>Execution Time</th>
                        <th>Request Time</th>
                    </tr>
                </thead>
                <tbody>
                    {tableBody}
                </tbody>
            </table>
        </div>
    );
};


export default Table;