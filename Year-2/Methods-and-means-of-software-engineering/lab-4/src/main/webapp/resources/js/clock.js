"use strict";

class Clock {
    constructor(params) {
        this.el = params;
        // el[0] - date
        // el[1] - time
        // el[2] - background
        this.setUpdateBackground();
    }

    startClock(sec = 1) {
        this.updateClock();
        this.tim_id = setInterval(this.updateClock.bind(this), sec * 1000);
    }

    updateClock() {
        function addZero(i) {
            return (i < 10) ? "0" + String(i) : String(i);
        }

        let date = new Date();

        let days = [date.getDate(), date.getMonth() + 1, date.getFullYear()];
        let time = [date.getHours(), date.getMinutes(), date.getSeconds()];

        time = time.map(element => addZero(element));

        this.el[1].innerHTML = time.join(":");
        this.el[0].innerHTML = days.join("/");

    }

    setUpdateBackground() {
        function getRandomColor() {
            let letters = '0123456789ABCDEF';
            let color = '#';
            for (let i = 0; i < 6; i++) {
                color += letters[Math.floor(Math.random() * 16)];
            }
            return color;
        }

        this.el[2].onclick = function () {
            this.style.backgroundColor = getRandomColor();
        }
    }
}

let c_d = document.querySelector(".clock__date");
let c_t = document.querySelector(".clock__time");
let c_b = document.querySelector(".clock__wrapper");
let p = [c_d, c_t, c_b];
let clock = new Clock(p);
clock.startClock(5);