let x, y, r;

//digit validation
function isNumeric(n) {
    return !isNaN(parseFloat(n)) && isFinite(n);
}

function isInRange(n, min, max) {
    return parseFloat(n) > parseFloat(min) && parseFloat(n) < parseFloat(max);
}

// form validation
function checkData(){
    return checkX() && checkY() && checkR();
}

function checkX(){
    let xf = document.querySelector(".form-coordinate-x__item input:checked");
    if (xf !== null) {
        x = parseFloat(xf.value.replace(",", "."));
        return true;
    }
    return false;
}

function checkY(){
    try {
        y = parseFloat(document.querySelector(".form-coordinate-y__item input[name='coordinate-y']").value.replace(",", "."));
        return isNumeric(y) ? isInRange(y, -5, 5) : false;
    }
    catch(err){
        return false;
    }
}

function checkR(){
    return r !== undefined;
}

// Points
const radiusPixels = 80;

function drawPoint(svg_x, svg_y, hit){
    let svg = document.querySelector(".svg-container__svg");
    svg.innerHTML += getHtmlCircleSvg(svg_x, svg_y, (hit ? "red" : "yellow"));
}

function loadPointsFromTable(currentRadius = 0){ //with svg coordinates
    let points = [];
    let tableRows = document.querySelectorAll(".table-body__row");
    if (tableRows !== null){
        tableRows.forEach(row => {
            let point = [];
            row.querySelectorAll("*").forEach(cell => {
                point.push(cell.innerHTML.trim());
            });
            if (currentRadius === 0) { currentRadius = parseFloat(point[2]);}
            point[0] = parseFloat(point[0].replace(",",".")) * radiusPixels / currentRadius;
            point[1] = -parseFloat(point[1].replace(",",".")) * radiusPixels / currentRadius;
            points.push(point);
        });
    }
    points.forEach(point => { drawPoint(point[0], point[1], (point[3] === 'true')); });
}

function deletePoints(){
    let points = document.querySelectorAll(".svg-container__svg .point-input");
    if (points !== null){
        points.forEach(point => {point.remove(); });
    }
}

function delayAndExecute(ms, action){
    const p = new Promise((resolve, reject) => {
        setTimeout(() => {
            resolve();
        }, ms);
    })
    p.then(() => {
        action();
    });
}

window.onload = function() {
    // Red effect on textfield
    let audio = new Audio("./js/media/ostanovites.mp3") // relative to index.jsp path
    let coordinateY = document.querySelector(".form-coordinate-y__item input[name='coordinate-y']");
    let pr_cord_length = 0;
    coordinateY.onkeyup = function() {
        y = this.value.replace(',', '.');

        if (String(y).length >= 16 && y.length > pr_cord_length && pr_cord_length < 16){
            console.log("played!" + String(y).length);
            audio.play();
        }

        if (y === "" || isNumeric(y) && isInRange(y, -5, 5)){
            setBackgroundColor(coordinateY, "white");
        }
        else {
            setBackgroundColor(coordinateY, '#ff5454');
        }
        pr_cord_length = String(y).length;
    };

    // Make from button a radio
    let radius_buttons = document.querySelectorAll('.form-radius__item input');
    radius_buttons.forEach(element => {
        element.onclick = function() {
            radius_buttons.forEach(element_inner => {
                setBackgroundColor(element_inner, "#ffffff");
            });
            setBackgroundColor(this, "silver");

            r = parseInt(this.value);
            deletePoints();
            loadPointsFromTable(r);
        };
    });

    // Points on svg 
    let svg = document.querySelector(".svg-container__svg");
    let pt = svg.createSVGPoint();
    // const svgViewBox = svg.viewBox.baseVal;

    function cursorPoint(evt){
        pt.x = evt.clientX;
        pt.y = evt.clientY;
        return pt.matrixTransform(svg.getScreenCTM().inverse());
    }
    // + send ajax request when clicking on svg
    svg.onclick = function(evt){
        let loc = cursorPoint(evt);
        if (r === undefined){
            alert("Radius is not set.")
        }
        else {
            //some code
            x = loc.x / radiusPixels * r;
            y = -loc.y / radiusPixels * r;
            $.ajax({
                type:"GET",
                url:"control",
                data: {
                    "source": "svg",
                    "value_R": r,
                    "value_X": x,
                    "value_Y": y,
                    "timezone_offset_minutes" : (new Date()).getTimezoneOffset()
                },
                contentType: "application/x-www-form-urlencoded; charset = UTF-8",
                success: function(data){
                    console.log(data);
                    document.querySelector(".table").innerHTML = data.htmlTable;
                    drawPoint(loc.x, loc.y, data.hit);
                },
                dataType:"json"
            });
        }
    };

    // Submit client button with ajax request
    document.querySelector(".client-button__submit").onclick = function(){
        if (pr_cord_length > 16){
            delayAndExecute(2000, () => {
                window.location.replace("https://www.youtube.com/watch?v=dQw4w9WgXcQ?autoplay=1");
            });
        }
        else if (!checkData()){
            alert("Data is Incorrect. Check it.");
        }
        else {
            $.ajax({
                type:"GET",
                url:"control",
                data: {
                    "source": "form-submit",
                    "value_R": r,
                    "value_X": x,
                    "value_Y": y,
                    "timezone_offset_minutes" : (new Date()).getTimezoneOffset()
                },
                contentType: "application/x-www-form-urlencoded; charset = UTF-8",
                success: function(data){
                    document.querySelector(".table").innerHTML = data.htmlTable;

                    let tableRow = document.querySelector(".table-body__row");
                    let point = [];
                    tableRow.querySelectorAll("*").forEach(cell => {
                        point.push(cell.innerHTML.trim());
                    });
                    point[0] = parseFloat(point[0].replace(",",".")) * radiusPixels / parseFloat(point[2]);
                    point[1] = -parseFloat(point[1].replace(",",".")) * radiusPixels / parseFloat(point[2]);
                    drawPoint(point[0], point[1], data.hit);
                },
                dataType:"json"
            });
        }
    };

    // Clear client button with ajax request
    document.querySelector(".client-button__clear").onclick = function(){
        $.ajax({
            type:"GET",
            url:"control",
            data: {
                "source": "form-clear"
            },
            success: function(data){
                $(".table-body").empty();
                deletePoints();
            },
            contentType: "application/x-www-form-urlencoded; charset = UTF-8",
            dataType:"text"
        });
    };

    // Load points on page
    loadPointsFromTable();
};