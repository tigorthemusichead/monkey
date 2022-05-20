let btn = document.querySelector('.button');
let isOn = 0;

const licenceText = `Соглашение по использованию
Приложение предназначено исключительно для совместного использования с оборудованием серии МАРТ, которое имеет такую возможность. 
Совместное использование не изменяет основных характеристик оборудования и не предоставляет материальной выгоды.
Никакие действия или бездействия приложения не могут быть истолкованы как основания по причинению, какого либо, вреда.
Приложение предоставляется по принципу «как есть».  Для ознакомления возможностей приложения отводится десять часов совместного использования с оборудованием или месяц после установки.
Дальнейшее использование приложения подтверждает, что оно полностью отвечает вашим ожиданиям и удовлетворяет вашим потребностям.
Не допускается вносить любые изменения в приложение, разбирать его код. Нарушение этого пункта приведет к немедленному расторжению соглашения.`

function turn_off(){
    let request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/off', false);
    request.onload = () => {
        if(request.readyState === 4 && request.status === 200) {
            let response = request.responseText;
            setSilentTimer(parseInt(response));
            return Number.parseInt(response);
        }
        else return -1;
    };
    request.send();
}

function manage_on(){
    let request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/manage-on', false);
    request.onload = () => {
        if(request.readyState === 4 && request.status === 200) {
            let response = request.responseText;
            return Number.parseInt(response);
        }
        else return -1;
    };
    request.send();
}

function manage_off(){
    let request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/manage-off?length:5000', false);
    request.onload = () => {
        if(request.readyState === 4 && request.status === 200){
            let response = request.responseText;
            return Number.parseInt(response);
        }
        else return -1;
    };
    request.send();
}

function getReady(){
    let request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/ready', false);
    request.onload = () => {
        if(request.readyState === 4 && request.status === 200){
            let response = request.responseText;
            return response;
        }
        else return -1;
    };
    request.send();
}

const timerCount = 10;
//Current data
const notFound = 'ПОДКЛЮЧИТЕ';
const notReady = 'ПОДОЖДИТЕ';
const ready = 'ГОТОВО';
const status = ['ПОДКЛЮЧИТЕ', 'ВОЗЬМИТЕ УПРАВЛЕНИЕ', 'ПОВЕРНИТЕ КЛЮЧ', 'ВКЛЮЧИТЕ', 'РАБОТАЕТ', 'ПАУЗА'];
let staticData = {
    current: {
        timerActive: 0,
        time: 0,
        name: null,
        state: 0,
        button: 0
    },
    timers: [{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null},{time:0, name:null}]
};

let moduleStatus = {};

//Status checking
function getStatus(){

    const faceState = document.querySelector('#state');
    const voltage = document.querySelector('.voltage');
    const error = document.querySelector('.error');
    const button = document.querySelector('.button');
    const offButton = document.querySelector('.button_turn_off');
    const buttonText = document.querySelector('.button_text');
    const notFoundCover = document.querySelector('.not_found-cover');
    const workingCover = document.querySelector('.working-cover');
    //timer loop setting up
    setInterval(()=> {
        faceState.innerText = status[staticData.current.state];
        faceState.style.color = '#6F7E6E';
        faceState.style.color = '#AF9B59';
        faceState.style.color = '#DD8C70';
        // Try to send a request
        let err = 1;
        try {
            let request = new XMLHttpRequest();
            request.open('GET', 'http://192.168.4.1/state', true);
            request.onload = () => {
                if (request.readyState === 4 && request.status === 200) {
                    err = 0;
                    let response = JSON.parse(request.responseText);
                    console.log(response);
                    moduleStatus = response;
                    staticData.current.state = +response.stage + 1;

                    notFoundCover.style.visibility = 'hidden';

                    if((staticData.current.state === 1)&&(moduleStatus.is_working === '1'))
                        workingCover.style.visibility = 'visible';
                    else{
                        workingCover.style.visibility = 'hidden';
                    }
                        

                    voltage.style.height = (moduleStatus.voltage / 1500 * 100) + "%";
                    if(+moduleStatus.voltage < 500) voltage.style.background ='#AF9B59';
                    else if(+moduleStatus.voltage < 900) voltage.style.background = '#6F7E6E';
                    else voltage.style.background = '#DD8C70';

                    if(+moduleStatus.on_error) error.style.display = "block";
                    else error.style.display = "none"

                    if((staticData.current.state === 2)||(staticData.current.state === 5)){
                        button.classList.add('button_inactive');
                    }
                    else
                        button.classList.remove('button_inactive');
                    if(staticData.current.state === 4) button.style.background = "#572c2c";
                    else button.style.background = "#757575";

                    if((staticData.current.state > 1)&&(staticData.current.state !== 4)){
                        offButton.classList.remove('button_inactive');
                    }      
                    else 
                        offButton.classList.add('button_inactive');

                    if(staticData.current.state === 1) buttonText.innerText = "ВЗЯТЬ";
                    else if(staticData.current.state === 3) buttonText.innerText = "ВКЛ";
                    else if(staticData.current.state === 4) buttonText.innerText = "ВЫКЛ";
                    else buttonText.innerText = "";
                }
                
            }
            request.send();
            // if something went wrong during sending
        } finally {
            if (err) {
                //condition to except blinking
                if(staticData.current.state === 0) {
                    notFoundCover.style.visibility = 'visible';
                    workingCover.style.visibility = 'hidden';
                }
                staticData.current.state = 0;
            }
        }
    }, 1000);

}

//Menu open and close
function toggleMenu(){
    const menu_box = document.querySelector('.menu_box');
    const box = document.querySelector('.box');
    const reset = document.querySelector('.button_turn_off');
    const lines = document.querySelectorAll('.menu-line');
    const button = document.querySelector('.menu');
    console.log(button.style);

    if(!staticData.current.button) {
        if (box.style.width === "100vw") {
            box.style.width = "0vw";
            menu_box.style.width = "100vw";
            reset.style.visibility = "hidden";
            lines[0].style.transform = "rotate(45deg)";
            lines[1].style.display = "none";
            lines[2].style.transform = "rotate(-45deg)";
            button.style.justifyContent = "center";
        } else {
            menu_box.style.width = "0vw";
            box.style.width = "100vw";
            reset.style.visibility = "visible";
            lines[0].style.transform = "rotate(0deg)";
            lines[1].style.width = "150px";
            lines[1].style.display = "block";
            lines[2].style.transform = "rotate(0deg)";
            button.style.justifyContent = "space-evenly";
        }
    }

}

const menu = document.querySelector(".menu");
menu.addEventListener('click', toggleMenu);


//Page onload
window.onload = ()=>{
    if(localStorage.getItem('licenceShown') === null)
        localStorage.setItem('licenceShown', '0');
    if(localStorage.getItem('staticData') === null){
        localStorage.setItem('staticData',  JSON.stringify(staticData));
    }
    
    const tempStatic = JSON.parse(localStorage.getItem('staticData'));
    staticData = JSON.parse(JSON.stringify(tempStatic));

    if(localStorage.getItem('licenceShown') === '0'){
        if(confirm(licenceText))
            localStorage.setItem('licenceShown', '1');
        else
            localStorage.setItem('licenceShown', '0');
    }
    for(let i = 0; i < timerCount; i++){
        const name = document.querySelector(`#name${i+1}`);
        const minInput = document.querySelector(`#min${i+1}`);
        const secInput = document.querySelector(`#sec${i+1}`);

        name.value = tempStatic.timers[i].name;
        minInput.value = Math.floor(parseInt(tempStatic.timers[i].time) / 60);
        secInput.value = parseInt(tempStatic.timers[i].time) % 60;
    }
    getStatus();
}


//Choosing timer

//Choosing timer Callback
function chooseTimer(num){
    return () => {
        staticData.current.timerActive = 1;
        const name = document.querySelector(`#name${num}`).value;
        let min = document.querySelector(`#min${num}`).value;
        let sec = document.querySelector(`#sec${num}`).value;
        if(sec.length === 1)
            sec = '0' + sec;
        if(min.length === 1)
            min = '0' + min;
        //Changing the timer name
        const faceName = document.querySelector('.chosen_timer');
        faceName.innerText = name;
        //Changing the time
        const faceTime = document.querySelector('.time_left');
        faceTime.innerText = min + ':' + sec;
        staticData.current.name = name;
        staticData.current.time = parseInt(min) * 60 + parseInt(sec);
        if (isNaN(staticData.current.time))
            staticData.current.time = 0;
    }
}
//Correcting data
function checkTimeData(num){
    return () => {
        const name = document.querySelector(`#name${num}`).value;
        const minInput = document.querySelector(`#min${num}`);
        const secInput = document.querySelector(`#sec${num}`);
        if(parseInt(minInput.value) > 10)
            minInput.value = 10;
        if(parseInt(secInput.value) > 59)
            secInput.value = 59;
        //Local Storage
        let tempStatic = JSON.parse(localStorage.getItem('staticData'));
        tempStatic.timers[num-1].time = parseInt(minInput.value)*60 + parseInt(secInput.value);
        tempStatic.timers[num-1].name = name;
        localStorage.setItem('staticData', JSON.stringify(tempStatic));
    };
}
//Setting event listeners
for(let i = 1; i < timerCount+1; i++){
    document.querySelector(`#choose${i}`).addEventListener('click', chooseTimer(i));
    document.querySelector(`#min${i}`).addEventListener('blur', checkTimeData(i));
    document.querySelector(`#sec${i}`).addEventListener('blur', checkTimeData(i));
}
//No timer
document.querySelector('#choose0').addEventListener('click', ()=>{
    const faceName = document.querySelector('.chosen_timer');
    faceName.innerText = 'нет таймера';
    const faceTime = document.querySelector('.time_left');
    faceTime.innerText = '--:--';
    staticData.current.timerActive = 0;
});

function setSilentTimer(len){ 
    const faceTime = document.querySelector('.time_left');
    len = Math.floor(len / 1000);
    let interval = setInterval(()=>{
        len -= 1;
        let min = Math.floor(len / 60).toString();
        if (min.length === 1)
            min = '0' + min;
        let sec = (len % 60).toString();
        if (sec.length === 1)
            sec = '0' + sec;
        faceTime.innerText = min + ':' + sec;
        //console.log("stage - " + staticData.current.stage);
        if(((len <= 0)&&(staticData.current.state === 5)) || staticData.current.state === 0){
            clearInterval(interval);
            getReady();
            if(staticData.current.time){
                min = Math.floor(staticData.current.time / 60).toString();
                sec = (staticData.current.time % 60).toString();
                if (sec.length === 1)
                    sec = '0' + sec;
                if (min.length === 1)
                    min = '0' + min;
                faceTime.innerText = min + ':' + sec;
            }
            else{
                sec = '--';
                min = '--';
                faceTime.innerText = min + ':' + sec;
            }
        }
    }, 1000);
}

//Timer Function
function setTimer(){
    let len = staticData.current.time;
    const faceTime = document.querySelector('.time_left');
    console.log('\n\n\n' + staticData.current.button + '\n\n\n');
    if(staticData.current.button === 0) {
        if (len && /*staticData.current.state && */staticData.current.timerActive) {
            manage_off();
            staticData.current.button = 1;
            button.classList.toggle('button_on');
            let interval = setInterval(function timer() {
                len--;
                let min = Math.floor(len / 60).toString();
                if (min.length === 1)
                    min = '0' + min;
                let sec = (len % 60).toString();
                if (sec.length === 1)
                    sec = '0' + sec;
                faceTime.innerText = min + ':' + sec;
                console.log(staticData.current.state);  
                if ((len < 1)||(staticData.current.button === 0)||(staticData.current.state === 0)) {
                    clearInterval(interval);
                    min = Math.floor(staticData.current.time / 60).toString();
                    sec = (staticData.current.time % 60).toString();
                    if (sec.length === 1)
                        sec = '0' + sec;
                    if (min.length === 1)
                        min = '0' + min;
                    faceTime.innerText = min + ':' + sec;
                    staticData.current.button = 0;
                    button.classList.toggle('button_on');
                    turn_off();
                }
            }, 1000);
        }
        else if(!staticData.current.timerActive){
            manage_off();
            staticData.current.button = 1;
            button.classList.toggle('button_on');
        }
    }
    else{
        staticData.current.button = 0;
        if(!staticData.current.timerActive)
            turn_off();
        button.classList.toggle('button_on');
    }
}

function manageButton(){
    if(staticData.current.state > 1){
        setTimer();
    }
    else{
        manage_on();
    }
}

function manageOffButton(){
    if(staticData.current.state === 2) manage_off();
    else turn_off();
}
const button = document.querySelector('.button');
const offButton = document.querySelector('.button_turn_off');
button.addEventListener('click', manageButton);
offButton.addEventListener('click', manageOffButton);