import React, { useEffect, useRef } from 'react';
import { useStore } from 'effector-react';

import { $radiusStore, addPoint, removePointById, clearPointStore, loadStore, setRadius } from '../../../state/state';
import { pointSave } from '../../../service/apiRequests';
import Point from '../../../model/point';
import userContext from '../../../model/context';

import "./index.scss";

const radiusSVG = 80;


const Graph = () => {
    const svg = useRef();
    const r = useStore($radiusStore);

    // unmount effects when react component changes
    useEffect(() => {
        return addPoint.watch(
            (point) => { drawPoint(point.coordinateX, point.coordinateY, point.radius, point.hit, point.pointId); }
        );
    });
    useEffect(() => {
        return removePointById.watch(
            (id) => { getPointById(id).remove(); }
        );
    });
    useEffect(() => {
        return clearPointStore.watch(
            () => { clearPoints(); }
        );
    });
    useEffect(() => {
        return loadStore.watch(
            (points) => {
                points.forEach(p => { drawPoint(p.coordinateX, p.coordinateY, p.radius, p.hit, p.pointId) });
            }
        );
    });
    useEffect(() => {
        return setRadius.watch(
            (newRadius) => { updatePoints(newRadius); }
        );
    });

    const handleSvgOnClick = (e) => {
        e.preventDefault();
        let pt = svg.current.createSVGPoint();
        pt.x = e.clientX;
        pt.y = e.clientY;
        let svgPt = pt.matrixTransform(svg.current.getScreenCTM().inverse());

        let p = new Point();
        // r - scale of svg, r - scale of radius
        p.coordinateX = svgPt.x / radiusSVG * (r * r);
        p.coordinateY = -svgPt.y / radiusSVG * (r * r);
        p.radius = r;
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
    };

    const updatePoints = (newRadius) => {
        document.querySelectorAll(".svg__point").forEach(p => {
            const scale = p.getAttribute("drawRadius") / newRadius;
            p.setAttribute("drawRadius", newRadius);
            p.setAttribute("cx", p.getAttribute("cx") * scale);
            p.setAttribute("cy", p.getAttribute("cy") * scale);
        });
    }

    const clearPoints = () => {
        document.querySelectorAll(".svg__point").forEach(p => p.remove());
    };

    const drawPoint = (pX, pY, radius, hit, id) => {
        // r - scale of svg, radius - point radius
        const svgX = pX * radiusSVG / (radius * r);
        const svgY = -pY * radiusSVG / (radius * r);
        let point = document.createElementNS("http://www.w3.org/2000/svg", "circle");
        const color = hit ? "red" : "yellow";
        point.setAttribute("cx", svgX);
        point.setAttribute("cy", svgY);
        point.setAttribute("fill", color);
        point.setAttribute("drawRadius", radius);
        point.setAttribute("pointId", id);
        // customization
        point.setAttribute("class", "svg__point");
        point.setAttribute("r", 2);
        point.setAttribute("stroke", "black");
        point.setAttribute("stroke-width", 0.5);
        svg.current.appendChild(point);
    };

    const getPointById = (id) => {
        return document.querySelector(`.svg__point[pointId='${id}']`);
    }


    return (
        <div className="graph">
            <svg ref={svg} onClick={handleSvgOnClick} class="graph__svg" viewBox="-100 -100 200 200" xmlns="http://www.w3.org/2000/svg">
                <defs>
                    <marker id='arrow-head' orient="auto"
                        markerWidth='2' markerHeight='4'
                        refX='0.1' refY='2'>
                        {/* <!-- triangle pointing right (+x) --> */}
                        <path d='M0,0 V4 L2,2 Z' fill="black" />
                    </marker>
                </defs>
                {/* <!--Figures--> */}
                <polygon points={`0 0, ${-radiusSVG / r}  0, 0 ${radiusSVG / 2 / r}`} fill="rgb(0, 160, 223)" />

                <rect x="0" y="0" width={radiusSVG / 2 / r} height={radiusSVG / r} fill="rgb(0, 160, 223)" />

                <path fill="rgb(0, 160, 223)" d={`M 0, ${-radiusSVG / r} a ${radiusSVG / r}, ${radiusSVG / r} 0 0 0 ${-radiusSVG / r},${radiusSVG / r} L 0 0 Z`} />
                <path
                    d="M -95 0, h 190"
                    stroke="black"
                    stroke-width="1"
                    marker-end="url(#arrow-head)" />
                <path
                    d="M 0 95, v -190"
                    stroke="black"
                    stroke-width="1"
                    marker-end="url(#arrow-head)" />
                {/* <!--Text--> */}
                <style>
                    {`
                        .inscription {
                            font-size:7px;
                            font-family:-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
                            font-style: normal;
                        }
                    `}
                </style>
                {/* <!--Points & dots--> */}
                <text x="92" y="-3" class="inscription">x</text>
                <text x="3" y="-92" class="inscription">y</text>
                <text x={36 / r} y={-3 / r} class="inscription">R/2</text>
                <text x={-47 / r} y={-3 / r} class="inscription">-R/2</text>
                <text x={3 / r} y={42 / r} class="inscription">-R/2</text>
                <text x={3 / r} y={-38 / r} class="inscription">R/2</text>
                <text x={78 / r} y={-3 / r} class="inscription">R</text>
                <text x={-84 / r} y={-3 / r} class="inscription">-R</text>
                <text x={3 / r} y={82 / r} class="inscription">-R</text>
                <text x={3 / r} y={-78 / r} class="inscription">R</text>
                <circle cx="0" cy="0" r="1.5" fill="black" />
                <circle cx="0" cy={40 / r} r="1.5" fill="black" />
                <circle cx="0" cy={-40 / r} r="1.5" fill="black" />
                <circle cx={40 / r} cy="0" r="1.5" fill="black" />
                <circle cx={-40 / r} cy="0" r="1.5" fill="black" />
                <circle cx={80 / r} cy="0" r="1.5" fill="black" />
                <circle cx={-80 / r} cy="0" r="1.5" fill="black" />
                <circle cx="0" cy={80 / r} r="1.5" fill="black" />
                <circle cx="0" cy={-80 / r} r="1.5" fill="black" />
            </svg>
        </div>
    )
}

export default Graph;