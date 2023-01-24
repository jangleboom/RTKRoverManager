#ifndef REBOOT_HTML_H
#define REBOOT_HTML_H


const char REBOOT_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <meta content="text/html" ; charset="UTF-8" ; http-equiv="content-type">
    <meta name="viewport" content="width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>RTK rover setup done</title>
    <style>
        body {
            background-color: #241E4E;
            color: #DFDFDF;
            font-family: Lato, Helvetica, Roboto, sans-serif;
            text-align: center;
            border: 1em;
        }

        .center {
            margin-left: auto;
            margin-right: auto;
        }

        progress {
            position: relative;
            accent-color: #399E5A;
        }
    </style>

    <script type="text/javascript">
        let duration = 5
        var timeleft = duration;
        var downloadTimer = setInterval(function() {
            
            if (timeleft <= -1) {
                clearInterval(downloadTimer);
            }

            document.getElementById("progressBar").value = duration - timeleft;
            let label = `in ${timeleft} second(s)`;
            document.getElementById("countdown").innerHTML = label;
            if ((timeleft) == -1) {
                document.getElementById("countdown").innerHTML = "(You can close this now.)";
            }
            timeleft -= 1;
        }, 1000);
    </script>
</head>

<body>
    <h2>RTK Rover</h2>
    <h3>setup done</h3>
    <br>
    device will restart and connect to SSID: %next_ssid%
    <br>
    <br>
    <progress value="0" max="5" id="progressBar"> </progress><br>
    <br>
    <br>
    <span id="countdown">in 5 second(s)</span> <br>

</body>

</html>

)rawliteral";


#endif /* HTML_H */


