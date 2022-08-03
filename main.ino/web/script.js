const timerMenuButton = document.querySelector('.timer-button-wrapper');
const timerMenuButtonOpen = document.querySelector('.open-timers');
const timerMenuButtonCLose = document.querySelector('.close-timers');
const timerWrapper = document.querySelector('.timer-wrapper');
const timerCheckboxes = document.querySelectorAll('.checkbox');
const headerInfo = document.querySelector('.header-box');
const time = document.querySelector('.time');
const timerForms = document.querySelectorAll('.timer');
const noTimerButton = document.querySelector('.no-timer');
const eraseTimersButton = document.querySelector('.erase');
const error = document.querySelector('.error');
const errorText = document.querySelector('.error-text');
const button =  document.querySelector('.button');
const statusText = document.querySelector('.status');
const cancelButton = document.querySelector('.cancel');
const voltageBar = document.querySelector('.voltage');

const licenceText = `Соглашение по использованию
Приложение предназначено исключительно для совместного использования с оборудованием серии МАРТ, которое имеет такую возможность. 
Совместное использование не изменяет основных характеристик оборудования и не предоставляет материальной выгоды.
Никакие действия или бездействия приложения не могут быть истолкованы как основания по причинению, какого либо, вреда.
Приложение предоставляется по принципу «как есть».  Для ознакомления возможностей приложения отводится десять часов совместного использования с оборудованием или месяц после установки.
Дальнейшее использование приложения подтверждает, что оно полностью отвечает вашим ожиданиям и удовлетворяет вашим потребностям.
Не допускается вносить любые изменения в приложение, разбирать его код. Нарушение этого пункта приведет к немедленному расторжению соглашения.`

const errorTexts = {
    "cancel-error-default": "Текущий процесс не возможно отменить.",
    "cancel-error-initial": "Прибор находится в изначальном состоянии,\n Отмена не возможна.\n Чтобы продолжить Возьмите внешнее управление\n или включите прибор вручную.",
    "cancel-error-pause": "Прибор находится в состоянии ПАУЗА,\n Отмена не возможна \n Дождитесь конца паузы чтобы продолжить использовать прибор.",
    "cancel-error-working": "При сбросе работы прибора возникла ошибка! \n Чтобы выключить прибор отключитесь от точки доступа.",
    "cancel-error-manage-off": "При сбросе внешнего управления возникла ошибка! \n Чтобы выключить прибор отключитесь от точки доступа.",
    "manage-error-key": "Не возможно взять внешнее управление, пока ключ находится в состоянии ВКЛ.\n Верните ключ в состояние ВЫКЛ чтобы продолжить!"
}

const statuses = ['Нет Связи', 'Готов', 'Поверните ключ', 'Включите', 'Работает', 'Пауза', 'Прибор занят'];
const buttons = [null, 'Взять Управление', null, 'Включить', 'Выключить', null, null];
const voltageColors = ['#A2E5A9', '#E5DAA2', '#DD6969'];

let timers = {
    activeIndex: 10,
    info: [
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: null, min: null, sec: null},
        {name: 'Без Таймера', min: null, sec: null}
    ]
}

let state = 0;
let voltage = 0;
let pauseTime = 0;
let errorMessage = "";

/**
 * UX/UI Code
 */

let touchStartY = 0;
let errorTimeout = setInterval(()=>{}, 0);

timerWrapper.addEventListener('touchstart', (e)=>{
    touchStartY = e.changedTouches[0].screenY
})

timerWrapper.addEventListener('touchend', (e)=>{
    if(e.changedTouches[0].screenY > touchStartY){
        timerMenuButtonOpen.style.display = 'inherit';
        timerMenuButtonCLose.style.display = 'none';
        //timerWrapper.style.top = 'calc(100vh - 170px)';
        timerWrapper.classList.remove('timer-wrapper-open');
    }
    else{
        timerMenuButtonOpen.style.display = 'none';
        timerMenuButtonCLose.style.display = 'inherit';
        //timerWrapper.style.top = '14px';
        timerWrapper.classList.add('timer-wrapper-open');
    }
})

/**
 * Timers logic
 */

let timerID;
//clearInterval(timer);

timerCheckboxes.forEach((checkbox, index) => {
    checkbox.addEventListener('click', ()=>{
        timerCheckboxes[timers.activeIndex]?.classList.remove('checkbox-active');
        checkbox.classList.add('checkbox-active');
        timers.activeIndex = index;
        setTimerInfo(index);
    })
});

timerForms.forEach((form, index) => {
    form.addEventListener('change', ()=>{
        timers.info[index] = {
            name: form.name.value,
            min: form.min.value < 10 ? form.min.value : 9,
            sec: form.sec.value < 60 ? form.sec.value : 59
        }
        setTimerForms();
        if(index === timers.activeIndex)
            setTimerInfo(index);
        localStorage.setItem('MART-timers', JSON.stringify(timers));
    })
})

noTimerButton.addEventListener('click', ()=>{
    timerCheckboxes[timers.activeIndex]?.classList.remove('checkbox-active');
    timers.activeIndex = 10;
    setTimerInfo(10);
})

eraseTimersButton.addEventListener('click', ()=>{
    timerCheckboxes[timers.activeIndex]?.classList.remove('checkbox-active');
    timers.activeIndex = 10;
    setTimerInfo(10);
    timers.info.forEach((timer, index) => {
        if(index < 10) timers.info[index] = {name: null, min: null, sec: null};
    })
    localStorage.setItem('MART-timers', timers);
    setTimerForms();
})

error.addEventListener('click', ()=>{
    if(error.classList[1] === 'error-active'){
        error.classList.remove('error-active');
        error.style.display = 'none';
    }
    else
        error.classList.add('error-active');
})

button.addEventListener('click', ()=>{
    let address = 'http://192.168.4.1';
    switch(state){
        case 1:
            address += '/manage-on';
            break;
        case 3:
            if(timers.activeIndex !== 10)
                address += `/manage-off?time=${msUtil(timers.info[timers.activeIndex])}`;
            else
                address += '/manage-off';
            break;
        case 4:
            address += '/cancel';
            break;
        default:
            address = null;
            break;
    }
    if(address){
        const request = new XMLHttpRequest();
        request.open("GET", address, false);
        request.onload = () => {
            if(request.readyState === 4 && request.status === 200){
                console.log(request.response);
                if(state === 3 && timers.activeIndex < 10){
                    setTimer(timers.info[timers.activeIndex].min, timers.info[timers.activeIndex].sec, 4, ()=>{
                            clearInterval(timerID);
                        setTimer(minSecUtil(pauseTime, 'min'), minSecUtil(pauseTime, 'sec'), 5, ()=>{
                            clearInterval(timerID);
                        });
                    });
                }
                else if(state === 3 && timers.activeIndex === 10){
                    setTimer(10, 0, 4, ()=>{
                        clearInterval(timerID)
                        setTimer(minSecUtil(pauseTime, 'min'), minSecUtil(pauseTime, 'sec'), 5, ()=>{
                            clearInterval(timerID);
                        });
                    });
                }
            }
            else if(request.readyState === 4 && request.status === 500){
                setError(errorTexts[request.response]);
            }
        }
        request.send();
    }
    getState();
})

cancelButton.addEventListener('click', ()=>{
    const request = new XMLHttpRequest();
    request.open("GET", 'http://192.168.4.1/cancel', false);
    request.onreadystatechange = () => {
        if(request.readyState === 4 && request.status === 200){
            console.log(request.response);
        }
        else if(request.readyState === 4 && request.status === 500){
            setError(errorTexts[request.response]);
        }
    }
    request.send();
    getState();
});

window.onload = async ()=>{
    if(localStorage.getItem('MART-timers')){
        timers = JSON.parse(localStorage.getItem('MART-timers'));
    }
    if(localStorage.getItem('NOTICE_AGREED') !== 'true'){
        const response = confirm(licenceText);
        if(response){
            localStorage.setItem('NOTICE_AGREED', 'true');
        }
    }
    setTimerForms();
    setStatus();
    setInterval(await getState, 2000);
}

function timeUtil(min, sec){
    if (!min && !sec)
        return '--:--';
    min = min ? `0${min}` : '00';
    sec = sec < 10 ? sec > 0 ? `0${sec}` : '00' : `${sec}`;
    return min + ':' + sec;
}

function msUtil({min, sec}){
    return min * 60000 + sec * 1000;
}

function minSecUtil(time, param){
    if(param === 'min')
        return Math.floor(time / 60000);
    else
        return Math.floor((time / 1000) % 60);
}

function setTimerInfo(index){
    console.log(timeUtil(timers.info[index].min, timers.info[index].sec));
    headerInfo.innerHTML = timers.info[index].name;
    time.innerText = timeUtil(timers.info[index].min, timers.info[index].sec);
}

function setTimerForms(){
    timerForms.forEach((form, index)=>{
        form.name.value = timers.info[index].name;
        form.min.value = timers.info[index].min;
        form.sec.value = timers.info[index].sec;
    })
}

function setStatus(){
    button.innerHTML = buttons[state];
    voltageBar.style.height = `${(voltage / 1500) * 100}%`;
    const colorIndex = voltage < 900 ? voltage > 500 ? 0 : 1 : 2;
    voltageBar.style.background = voltageColors[colorIndex];
    if([0, 2, 5, -1, 6].includes(state)) button.style.display = 'none';
    else button.style.display = 'inherit';
    if(errorMessage === "module-error"){
        statusText.innerText = "Ошибка железа"
        statusText.style.background = "#EA6A6A";
    }
    else{
        statusText.innerText = statuses[state];
        statusText.style.background = "#81AFF5";
    }
}

function setError(errorMessage){
    error.style.display = 'inherit';
    errorText.innerHTML = errorMessage;
    clearTimeout(errorTimeout);
    errorTimeout = setTimeout(()=>{
        error.style.display = 'none';
        errorText.innerHTML = null;

    }, 20000);
}

function setTimer(min, sec, normalState, action = ()=>{}){
    timerID = setInterval(()=>{
        if(sec > 0){
            sec --;
        }
        else if(min > 0){
            sec = 59;
            min --;
        }
        else{
            setTimerInfo(timers.activeIndex);
            getState().then(action);
            //setTimeout(action, 1000);
        }

        if(state !== normalState){
            setTimerInfo(timers.activeIndex);
            getState().then(action);
            //setTimeout(action, 1000);
        }

        time.innerHTML = timeUtil(min, sec);
    }, 1000);
}

async function getState(){
    return new Promise(function (resolve, reject) {
        makeRequest('GET', 'http://192.168.4.1/state')
            .then(response => JSON.parse(response))
            .then((response) => {
                console.log(response);
                if([6, 4].includes(state) && +response.state === 5) {
                    clearInterval(timerID);
                    setTimer(minSecUtil(+response.pausetime, 'min'), minSecUtil(+response.pausetime, 'sec'), 5, () => {
                        clearInterval(timerID);
                    });
                }

                if(state !== +response.state) setStatus();

                if(response.error === "hardware-stop" && errorMessage !== "hardware-stop"){
                    setStatus();
                    setError(`Работа прибора была остановлена вручную или в результате ошибки.\n Время работы прибора - ${minSecUtil(+response["lasted_working"], 'min')} минут ${minSecUtil(+response["lasted_working"], 'sec')} секунд`);
                }

                state = +response.state;
                voltage = +response.voltage;
                pauseTime = +response.pausetime;
                errorMessage = response.error;

                setStatus();
                resolve(true);
            })
            .catch((err)=>{
                setError("Потеряна связь с прибором!");
                state = 0;
                voltage = 0;
                pauseTime = 0;
                setStatus();
                resolve(false);
            })
    });
    /*const request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/state', false);

    request.addEventListener('loadend',  () => {
        if (request.readyState === 4 && request.status === 200) {
            const response = JSON.parse(request.response);
            state = +response.state;
            voltage = +response.voltage;
            pauseTime = +response.pausetime;
        } else {
            setError("Потеряна связь с прибором!");
            state = 0;
            voltage = 0;
            pauseTime = 0;
        }
        setStatus();
    })

    request.addEventListener('error', (error) => {
        setError(error + "\nОтвет не получен. Потеряна связь с прибором!");
        state = 0;
        voltage = 0;
        setStatus();
    })
    request.send();

     */
}

function makeRequest(method, url) {
    return new Promise(function (resolve, reject) {
        let xhr = new XMLHttpRequest();
        xhr.open(method, url);
        xhr.onload = function () {
            if (this.status >= 200 && this.status < 300) {
                resolve(xhr.response);
            } else {
                reject({
                    status: this.status,
                    statusText: xhr.statusText
                });
            }
        };
        xhr.onerror = function () {
            reject({
                status: this.status,
                statusText: xhr.statusText
            });
        };
        xhr.send();
    });
}