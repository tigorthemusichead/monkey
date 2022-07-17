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

/**
 * UX/UI Code
 */

let touchStartY = 0;

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

window.onload = ()=>{
    if(localStorage.getItem('MART-timers')){
        timers = JSON.parse(localStorage.getItem('MART-timers'));
    }
    setTimerForms();
}

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

function timeUtil(min, sec){
    if (!min && !sec)
        return '--:--';
    min = min ? `0${min}` : '00';
    sec = sec < 10 ? sec > 0 ? `0${sec}` : '00' : `${sec}`;
    return min + ':' + sec;
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