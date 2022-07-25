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

const licenceText = `Соглашение по использованию
Приложение предназначено исключительно для совместного использования с оборудованием серии МАРТ, которое имеет такую возможность. 
Совместное использование не изменяет основных характеристик оборудования и не предоставляет материальной выгоды.
Никакие действия или бездействия приложения не могут быть истолкованы как основания по причинению, какого либо, вреда.
Приложение предоставляется по принципу «как есть».  Для ознакомления возможностей приложения отводится десять часов совместного использования с оборудованием или месяц после установки.
Дальнейшее использование приложения подтверждает, что оно полностью отвечает вашим ожиданиям и удовлетворяет вашим потребностям.
Не допускается вносить любые изменения в приложение, разбирать его код. Нарушение этого пункта приведет к немедленному расторжению соглашения.`

const statuses = ['Нет Связи', 'Готов', 'Поверните ключ', 'Включите', 'Работает', 'Пауза'];
const buttons = [null, 'Взять Управление', null, 'Включить', 'Выключить', null];
const voltageColors = ['#A2E5A9', '#E5DAA2', '#DD6969'];

let timers = {
    activeIndex: null,
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
        timerWrapper.style.top = 'calc(100vh - 170px)';
    }
    else{
        timerMenuButtonOpen.style.display = 'none';
        timerMenuButtonCLose.style.display = 'inherit';
        timerWrapper.style.top = '14px';
    }
})

/**
 * Timers logic
 */

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
    timers.activeIndex = null;
    setTimerInfo(10);
})

eraseTimersButton.addEventListener('click', ()=>{
    timerCheckboxes[timers.activeIndex]?.classList.remove('checkbox-active');
    timers.activeIndex = null;
    setTimerInfo(10);
    timers.info.forEach((timer, index) => {
        if(index < 10) timers.info[index] = {name: null, min: null, sec: null};
    })
    localStorage.setItem('MART-timers', timers);
    setTimerForms();
})

error.addEventListener('click', ()=>{
    error.classList.toggle('error-active');
})

button.addEventListener('click', ()=>{
    let address = 'http://192.168.4.1';
    switch(state){
        case 1:
            address += '/manage-on';
            break;
        case 3:
            if(timers.activeIndex !== null)
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
            if(request.readyState == 4 && request.status == 200){
                console.log(request.response);
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
    }
    request.send();
    getState();
});

window.onload = ()=>{
    if(localStorage.getItem('MART-timers')){
        timers = JSON.parse(localStorage.getItem('MART-timers'));
    }
    setTimerForms();
    setInterval(getState, 2000);
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

function setTimerInfo(index){
    headerInfo.innerHTML = timers.info[index].name || "Безымянный таймер";
    time.innerHTML = timeUtil(timers.info[index].min, timers.info[index].sec);
}

function setTimerForms(){
    timerForms.forEach((form, index)=>{
        form.name.value = timers.info[index].name;
        form.min.value = timers.info[index].min;
        form.sec.value = timers.info[index].sec;
    })
}

function setStatus(){
    statusText.innerText = statuses[state];
    button.innerHTML = buttons[state];
    if([0, 2, 5, -1].includes(state)) button.style.display = 'none';
    else button.style.display = 'inherit';
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

function getState(){
    const request = new XMLHttpRequest();
    request.open('GET', 'http://192.168.4.1/state', false);

    request.addEventListener('loadend',  () => {
        if (request.readyState === 4 && request.status === 200) {
            const response = JSON.parse(request.response);
            state = +response.state;
            voltage = +response.voltage;
        } else {
            setError("Потеряна связь с прибором!");
            state = 0;
            voltage = 0;
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

}