<?php
    function checkHit($value_x, $value_y, $value_r) {
        //check triangle
        $inner_first = function($value_x, $value_y, $value_r) {
            if ($value_x >= -$value_r && $value_x <= 0){
                return $value_y <= $value_x + $value_r  && $value_y >= 0;
            }
            return false;
        };
        //check rectangle
        $inner_second = function($value_x, $value_y, $value_r) {
            if ($value_x >= -$value_r / 2 && $value_x <= 0){
                return $value_y <= 0 && $value_y >= -$value_r;
            }
            return false;
        };
        //check circle
        $inner_third = function($value_x, $value_y, $value_r) {
            if ($value_x >= 0 && $value_x <= $value_r){
                return $value_y <= 0 && (pow($value_x, 2) + pow($value_y, 2) <= pow($value_r, 2));
            }
            return false;
        };
        return ($inner_first($value_x, $value_y, $value_r) || $inner_second($value_x, $value_y, $value_r) || $inner_third($value_x, $value_y, $value_r));
    };
    
    function getDatetimeWithOffset($offset) {
        $timezone_name = timezone_name_from_abbr("", -$offset*60, false);
        $dt = new DateTime("now", new DateTimeZone($timezone_name));
        // $timestamp = time();
        // $dt->setTimestamp($timestamp);
        return $dt->format("Y-m-d H:i:s");
    };

    if (isset($_GET["value_R"])){
        $start = microtime(true);
        $x = $_GET["value_X"];
        $y = $_GET["value_Y"];
        $r = $_GET["value_R"];
        $hit = checkHit($x, $y, $r);
        $current_time = getDatetimeWithOffset($_GET["timezone_offset_minutes"]);
        $script_time = (microtime(true) - $start);

        // echo json_encode(array(
        //     "value_X" => $x,
        //     "value_Y" => $y,
        //     "value_R" => $r,
        //     "value_hit" => $hit,
        //     "current_time" => $current_time,
        //     "script_time_seconds" => $script_time
        // ));
        echo("
        {
            \"value_X\": \"$x\",
            \"value_Y\": \"$y\",
            \"value_R\": \"$r\",
            \"value_hit\": \"$hit\",
            \"current_time\": \"$current_time\",
            \"script_time_seconds\": \"$script_time\"
        }
        ");
    }