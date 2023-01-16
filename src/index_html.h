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
        :root {
            --RUSSIAN_VIOLETT: #241E4E;
            --GREEN_COLOR: #399E5A;
            --RED_COLOR: #CE6C47;
            --GAINSBORO: #DFDFDF;
            --TEAL_BLUE: #40798C;
            --BLACK: #000000;
        }

        body {
            background-color: var(--RUSSIAN_VIOLETT);
            color: var(--GAINSBORO);
            font-family: Lato, Helvetica, Roboto, sans-serif;
            text-align: center;
            border: 1em;
        }

        .table {
            margin-left: auto;
            margin-right: auto;
        }

        .button {
            border-radius: 4px;
            border: none;
            padding: 15px 30px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 13px;
        }

        .blue {
            background-color: var(--TEAL_BLUE);
            color: var(--RUSSIAN_VIOLETT);
        }

        .green {
            background-color: var(--GREEN_COLOR);
            color: var(--RUSSIAN_VIOLETT);
        }

        .red {
            background-color: var(--RED_COLOR);
            color: var(--RUSSIAN_VIOLETT);
        }

        .text_field {
            border-radius: 4px;
            border: none; // color: black;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 13px;
            width: 200px;
            color: black;
        }

        /* set the opacity of placeholder */

        ::placeholder {
            opacity: 1;
        }

        :focus::placeholder {
            opacity: 0.1;
        }
    </style>
</head>
<script>
function checkVariables()
{
    // check if variable exists
    let ssid            = "%ssid%";
    let password        = "%password%";
    let device_name     = "%device_name%";
    let caster_host     = "%caster_host%";
    let caster_port     = "%caster_port%";
    let mount_point     = "%mount_point%";
    let caster_user     = "%caster_user%";


    if (ssid) 
    {
        document.getElementsByName("ssid")[0].placeholder = ssid;
        document.getElementsByName("password")[0].style.color = "black";
    } 
    else 
    {
        document.getElementsByName("ssid")[0].placeholder = "Enter WiFi SSID";
        document.getElementsByName("ssid")[0].style.color = "grey";
    }

    if (password) 
    {
        document.getElementsByName("password")[0].placeholder = password;
        document.getElementsByName("password")[0].style.color = "black";
    } 
    else 
    {
        document.getElementsByName("password")[0].placeholder = "Enter WiFi password";
        document.getElementsByName("password")[0].style.color = "grey";
    }

    if (device_name) 
    {
        document.getElementsByName("device_name")[0].placeholder = device_name;
        document.getElementsByName("device_name")[0].style.color = "black";
    } 
    else 
    {
        document.getElementsByName("device_name")[0].placeholder = "Enter a custom host name";
        document.getElementsByName("device_name")[0].style.color = "grey";
    }

    if (caster_host) 
    {
        document.getElementsByName("caster_host")[0].placeholder = caster_host;
        document.getElementsByName("caster_host")[0].style.color = "black";
    } 
    else 
    {
        document.getElementsByName("caster_host")[0].placeholder = "Enter caster host";
        document.getElementsByName("caster_host")[0].style.color = "grey";
    }

    if (caster_port) 
    {
        document.getElementsByName("caster_port")[0].placeholder = caster_port;
        document.getElementsByName("caster_port")[0].style.color = "black";
    } 
    else 
    {
        document.getElementsByName("caster_port")[0].placeholder = "Enter caster port";
        document.getElementsByName("caster_port")[0].style.color = "grey";
    }

    if (mount_point) 
    {
        document.getElementsByName("mount_point")[0].placeholder = mount_point;
        document.getElementsByName("mount_point")[0].style.color = "black";
    } 
    else 
    {
        document.getElementsByName("mount_point")[0].placeholder = "Enter caster mountpoint";
        document.getElementsByName("mount_point")[0].style.color = "grey";
    }

    if (caster_user) 
    {
        document.getElementsByName("caster_user")[0].placeholder = caster_user;
        document.getElementsByName("caster_user")[0].style.color = "black";
    } 
    else 
    {
        document.getElementsByName("caster_user")[0].placeholder = "Enter caster user email";
        document.getElementsByName("caster_user")[0].style.color = "grey";
    }

    console.log(ssid);
    console.log(password);
    console.log(device_name);
    console.log(mount_point);
    console.log(caster_user);
}
</script>

<body onload="checkVariables();">

    <form id="Form1" onsubmit="return confirm('Restart the ESP32 by pressing the Reboot button for your changes to take effect!');" action='actionUpdateData' method='post' target="hidden-form"></form>
    <form id="Form2" onsubmit="return confirm('Are you sure? All saved files will be deleted (Wifi, BNO080 and RTK config)');" action='actionWipeData' method='post' target="hidden-form"></form>
    <form id="Form3" onsubmit="return confirm('Connection will be lost during reboot. This server appears in AP-Mode only (push wipe button on device to force this)!');" action='actionRebootESP32' method='post' target="hidden-form"></form>
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
                        <input class="text_field" form="Form1" type="text" maxlength="30" name="ssid" placeholder={{ssid}}>
                    </td>
                </tr>
                <tr>
                    <td style="text-align:left;">Password:</td>
                    <td>
                        <input class="text_field" form="Form1" type="text" maxlength="30" name="password" placeholder={{passwor}}d>
                    </td>

                </tr>
                <tr>
                    <td colspan=2>
                        <h3>RTK rover setup</h3>
                    </td>
                    <tr>
                        <td style="text-align:left;">Device name:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" name="device_name" placeholder={{device_name}}>
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align:left;">Caster host:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="caster_host" placeholder={{caster_host}}>
                        </td>
                    </tr>
                        <tr>
                        <td style="text-align:left;">Caster port:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="caster_port" placeholder={{caster_port}}>
                        </td>

                    </tr>
                    <tr>
                        <td style="text-align:left;">Mount point:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="mount_point" placeholder={{mount_point}}>
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align:left;">Caster user:</td>
                        <td>
                            <input class="text_field" form="Form1" type="text" maxlength="30" name="caster_user" placeholder={{caster_user}}>
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


