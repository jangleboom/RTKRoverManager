#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <meta content="text/html" ; charset="UTF-8" ; http-equiv="content-type">
    <meta name="viewport" content="width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>RTK rover setup</title>
    <style>
        body 
        {
            background-color: #241E4E;
            background-image: radial-gradient(#15112C, #241E4E, #312966);
            color: #DFDFDF;
            font-family: Lato, Helvetica, Roboto, sans-serif;
            text-align: center;
            border: 1em;
        }

        .table 
        {
            margin-left: auto;
            margin-right: auto;
        }

        .button 
        {
            border-radius: 4px;
            border: none;
            padding: 13px 18px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 13px;
        }
        .blue 
        {
            background-color: #40798C;
            color: #241E4E;
        }
        .green 
        {
            background-color: #399E5A;
            color: #241E4E;
        }
        .red
        {
            background-color: #CE6C47;
            color: #241E4E;
        }

        .text_field 
        {
            border-radius: 4px;
            border: none;
            color: black;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 13px;
        }

        ::placeholder 
        {
            color: black;
            opacity: 1;
            transition: opacity 1s;
        }

        :focus::placeholder 
        {
            opacity: 0.1;
        }
    </style>
</head>
<script>
</script>

<body>

    <form id="Form1" onsubmit="return confirm('Restart the ESP32 by pressing the Reboot button for your changes to take effect!');" action='actionUpdateData' method='post' target="hidden-form"></form>
    <form id="Form2" onsubmit="return confirm('Are you sure? All saved SPIFFS files will be deleted (Wifi, BNO080 and RTK config)');" action='actionWipeData' method='post' target="hidden-form"></form>
    <form id="Form3" onsubmit="return confirm('Connection will be lost during reboot. This server appears in AP-Mode only (push wipe button on device to force this)!');" action='actionRebootESP32' method='post' target="hidden-form"></form>
    <input form="Form1" type="hidden" id="radio_state" value=%location_method%>
    <p>
        <table class="table">
            <tr>
                <td colspan=2>
                    <h2>RTK Rover</h2>
                </td>
                <td colspan=2></td>
            </tr>
            <tr>
                <td colspan=2>
                    <h3>WiFi credentials</h3>
                </td>
                <tr>
                    <td style="text-align:left;">SSID:</td>
                    <td>
                        <input class="text_field" form="Form1" type="text" maxlength="30" name="ssid" placeholder=%ssid% style="text-align:center;">
                    </td>
                </tr>
                <tr>
                    <td style="text-align:left;">Password:</td>
                    <td>
                        <input class="text_field" form="Form1" type="text" maxlength="30" name="password" placeholder=%password% style="text-align:center;">
                    </td>

                </tr>
                <tr>
                    <td colspan=2>
                        <h3>RTK rover setup</h3>
                    </td>
                    <tr>
                        <td style="text-align:left;">Device name:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" name="device_name" placeholder=%device_name%>
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align:left;">Caster host:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="caster_host" placeholder=%caster_host% style="text-align:center;">
                        </td>
                    </tr>
                        <tr>
                        <td style="text-align:left;">Caster port:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="caster_port" placeholder=%caster_port% style="text-align:center;">
                        </td>

                    </tr>
                    <tr>
                        <td style="text-align:left;">Mount point:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="mount_point" placeholder=%mount_point% style="text-align:center;">
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align:left;">Caster user:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="caster_user" placeholder=%caster_user% style="text-align:center;">
                        </td>
                    </tr>
        </table>
    </p>
    <br>
    <br>
    <div>
        <input type="submit" form="Form1" class="button green" formaction="/actionUpdateData" value="Save" id="save_button" name="save_button" />
        <input type="submit" form="Form3" class="button green" formaction="/actionRebootESP32" value="Reboot" id="reboot_button" name="reboot_button" />
        <input type="reset" form="Form1" class="button blue" value="Cancel" />
        <input type="submit" form="Form2" class="button red" formaction="/actionWipeData" value="Wipe" id="wipe_button" name="wipe_button" />
    </div>
</body>

</html>
)rawliteral";

#endif /* INDEX_HTML_H */


