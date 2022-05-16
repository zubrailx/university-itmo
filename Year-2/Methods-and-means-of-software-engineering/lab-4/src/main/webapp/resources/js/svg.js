let $svg = document.querySelector(".svg-container__svg");
let $radius = document.querySelector(".form-radius__input > input");

const svg_r = 80;

let $svg_x_input = document.querySelector(".svg-container__input-x");
let $svg_y_input = document.querySelector(".svg-container__input-y");
let $svg_button = document.querySelector(".svg-container__button");


function toFormCoordinates(point) {
    point.x = point.x / svg_r * parseFloat($radius.value.replace(",", "."));
    point.y = -point.y / svg_r * parseFloat($radius.value.replace(",", "."));
    return point;
}

function toSvgCoordinates(point) {
    point.x = point.x * svg_r / parseFloat($radius.value.replace(",", "."));
    point.y = -point.y * svg_r / parseFloat($radius.value.replace(",", "."));
    return point;
}
function isNumeric(n) {
    return !isNaN(parseFloat(n)) && isFinite(n);
}

function validateForm() {
    try {
        let flagX = (isNumeric($svg_x_input.value));
        let flagY = (isNumeric($svg_y_input.value));
        let r = parseFloat($radius.value.replace(",", "."));
        let flagR = (isNumeric(r));
        return flagX && flagY && flagR;
    } catch (exc) {
        return false;
    }
}

function getHtmlCircleSvg(svg_x, svg_y, color){
    return `<circle class="point-input" cx="${svg_x}" cy="${svg_y}" r="2" fill="${color}" stroke="black" stroke-width="0.5"/>`;
}

function drawPoint(svg_x, svg_y, hit){
    let svg = document.querySelector(".svg-container__svg");
    svg.innerHTML += getHtmlCircleSvg(svg_x, svg_y, (hit ? "red" : "yellow"));
}

function loadPointsFromTable(){
    let $table_rows = document.querySelector(".table-container__table").tBodies[0].rows;
    if ($table_rows[0].cells[0].innerText === '') return;
    let points = [];
    for (let i = 0; i < $table_rows.length; ++i){
        let point = {};
        point.x = parseFloat($table_rows[i].cells[0].innerText.replace(",","."));
        point.y = parseFloat($table_rows[i].cells[1].innerText.replace(",","."));
        point.hit = $table_rows[i].cells[3].innerText.trim() === 'true';
        point = toSvgCoordinates(point);
        points.push(point);
        console.log(i);
    }
    points.forEach(point => { drawPoint(point.x, point.y, point.hit); });
}

function deletePoints(){
    let points = document.querySelectorAll(".svg-container__svg .point-input");
    if (points !== null){
        points.forEach(point => {point.remove(); });
    }
}

//set point
let pt = $svg.createSVGPoint();  // Created once for document
$svg.onclick = (evt) => {
    pt.x = evt.clientX;
    pt.y = evt.clientY;

    let point = pt.matrixTransform($svg.getScreenCTM().inverse());

    point = toFormCoordinates(point);

    $svg_x_input.value = point.x;
    $svg_y_input.value = point.y;

    if (validateForm()) {
        $svg_button.click();
    } else {
        alert("Form data is invalid");
    }
}

let $form_radius_input = document.querySelector(".form-radius__input > input");
$form_radius_input.onchange = () => {
    deletePoints();
    loadPointsFromTable();
}


// main
loadPointsFromTable();

