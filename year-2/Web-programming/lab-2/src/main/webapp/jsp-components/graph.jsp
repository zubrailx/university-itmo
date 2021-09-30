<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<div class="svg-container">
    <svg class="svg-container__svg" viewBox="-100 -100 200 200" xmlns="http://www.w3.org/2000/svg" >
        <defs>
            <marker id='arrow-head' orient="auto"
                    markerWidth='2' markerHeight='4'
                    refX='0.1' refY='2'>
                <!-- triangle pointing right (+x) -->
                <path d='M0,0 V4 L2,2 Z' fill="black"></path>
            </marker>
        </defs>
        <!--Figures-->
        <polygon points="0 0, 0 -40, -80 0" fill="rgb(0, 160, 223)"></polygon>
        <rect x="0" y="0" width="80" height="80" fill="rgb(0, 160, 223)"></rect>
        <path fill="rgb(0, 160, 223)" d="M 0,40 a 40,40 0 0 1 -40,-40 L 0 0 Z"></path>
        <path
                d="M -95 0, h 190"
                stroke="black"
                stroke-width="1"
                marker-end="url(#arrow-head)"></path>
        <path
                d="M 0 95, v -190"
                stroke="black"
                stroke-width="1"
                marker-end="url(#arrow-head)"></path>
        <!--Text-->
        <style>
            .inscription{
                font-size:7px;
                font-family:-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
                font-style: normal;
            }
        </style>
        <!--Points & dots-->
        <text x="92" y="-3" class="inscription">x</text>
        <text x="3" y="-92" class="inscription">y</text>
        <text x="36" y="-3" class="inscription">R/2</text>
        <text x="-47" y="-3" class="inscription">-R/2</text>
        <text x="3" y="42" class="inscription">-R/2</text>
        <text x="3" y="-38" class="inscription">R/2</text>
        <text x="78" y="-3" class="inscription">R</text>
        <text x="-84" y="-3" class="inscription">-R</text>
        <text x="3" y="82" class="inscription">-R</text>
        <text x="3" y="-78" class="inscription">R</text>
        <circle cx="0" cy="0" r="1.5" fill="black"></circle>
        <circle cx="0" cy="40" r="1.5" fill="black"></circle>
        <circle cx="0" cy="-40" r="1.5" fill="black"></circle>
        <circle cx="40" cy="0" r="1.5" fill="black"></circle>
        <circle cx="-40" cy="0" r="1.5" fill="black"></circle>
        <circle cx="80" cy="0" r="1.5" fill="black"></circle>
        <circle cx="-80" cy="0" r="1.5" fill="black"></circle>
        <circle cx="0" cy="80" r="1.5" fill="black"></circle>
        <circle cx="0" cy="-80" r="1.5" fill="black"></circle>
    </svg>
</div>
