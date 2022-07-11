#pragma once
#include <string>
#include <fstream>

ifstream index("web/index.html");
string line_string;
string text_string;

while (getline (index, line_string))
{
  text_string += line_string;
}

char text = "=====(<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <style>.kek{
    color: red;
}</style></head>
<body>
    <button class="kek">
        lol
    </button>
    <script> const kek = document.querySelector('.kek');
kek.addEventListener('click', ()=>{
    alert(1);
})</script>
    <script src="script.js"></script>
</body>
</html>)=====";

char web[] = R text;
