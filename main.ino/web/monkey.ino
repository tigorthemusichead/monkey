#pragma once

char web[] = R"=====(<!doctype html>
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
</body>
</html>)=====";