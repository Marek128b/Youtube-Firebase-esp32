// Import the functions you need from the SDKs you need
import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.21.0/firebase-app.js'
import { getDatabase, ref, onValue, set, get, update } from "https://www.gstatic.com/firebasejs/9.21.0/firebase-database.js";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyCFGIi20VOO_krrHcP06PwChzrvF-g1knw",
    authDomain: "test-35c8b.firebaseapp.com",
    databaseURL: "https://test-35c8b-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "test-35c8b",
    storageBucket: "test-35c8b.appspot.com",
    messagingSenderId: "827625103040",
    appId: "1:827625103040:web:7a7f79bcb9d3b1a30e1725"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

//initialize rtdb
const db = getDatabase();

//get database data and print to element
const pot1 = document.getElementById("pot1Value");
const starCountRef = ref(db, "/pot1");
onValue(starCountRef, (snapshot) => {
    const data = snapshot.val();
    console.log(data);
    pot1.innerText = "Pot1 Value: " + String(data);
});

//add button event
const button1 = document.getElementById("buttonLED1");
let toggle1 = true;
onValue(ref(db, "/led1"), (snapshot) => {
    const data = snapshot.val();
    console.log(data);
    button1.innerText = toggle1 ? "LED1: on" : "LED1: off";
});
button1.addEventListener("click", function () {
    console.log("clicked Button1");
    if (toggle1) {
        this.style.backgroundColor = "rgb(0,150,20)";
    } else {
        this.style.backgroundColor = "rgb(150,0,20)";
    }

    // Set the value to the database
    update(ref(db), {
        "led1": toggle1
    });

    //change variable 
    toggle1 = !toggle1;
});

