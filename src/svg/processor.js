console.log("added");
function add_text()
{
    var t1 = document.querySelector("#text_fetch");
    var t2 = document.querySelector("#text_decode");
    var t3 = document.querySelector("#text_alu");
    var t4 = document.querySelector("#text_mem");
    var t5 = document.querySelector("#text_write");
    t1.textContent = "Fetch Stage";
    t2.textContent = "Decode Stage";
    t3.textContent = "ALU Stage";
    t4.textContent = "Memory Stage";
    t5.textContent = "WriteBack Stage";

    setTimeout(function() {location.reload(true)},500);
}