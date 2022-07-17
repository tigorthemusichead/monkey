const timerMenuButton = document.querySelector('.timer-button-wrapper');
const timerMenuButtonOpen = document.querySelector('.open-timers');
const timerMenuButtonCLose = document.querySelector('.close-timers');
const timerWrapper = document.querySelector('.timer-wrapper');
const timerCheckboxes = document.querySelectorAll('.checkbox');

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

timerCheckboxes.forEach((checkbox, index) => {
    checkbox.addEventListener('click', ()=>{
        timerCheckboxes.forEach(checkbox => {
            checkbox.classList.remove('checkbox-active');
        });
        checkbox.classList.add('checkbox-active');
    })
});

window.onload = ()=>{
    const elem = document.documentElement;
    if (elem.requestFullscreen)
        elem.requestFullscreen()
}