document.addEventListener("DOMContentLoaded", function() {
    function goToAboutUs() {
        let confirmation = confirm("Are you sure you want to go to the About page?");
        if (confirmation) {
            window.location.href = "./about.html";
        }
    }

    function playAudio() {
        let audio = document.getElementById("audio");
        audio.style.display = "block";
        audio.play();
    }

    function playVideo() {
        let video = document.getElementById("video");
        video.style.display = "block";
        video.play();
    }

    window.goToAboutUs = goToAboutUs;
    window.playAudio = playAudio;
    window.playVideo = playVideo;
});
