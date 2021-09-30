function getHtmlCircleSvg(svg_x, svg_y, color){
    return `<circle class="point-input" cx="${svg_x}" cy="${svg_y}" r="2" fill="${color}" stroke="black" stroke-width="0.5"/>`;
}

function setBackgroundColor(node, color){
    node.style.backgroundColor = color;
}