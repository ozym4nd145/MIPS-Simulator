function refresh(node) {
    var delay = 500; // gap in Milli Seconds;

    (function start_refresh() {
        var address;
        if (node.src.indexOf('?') > -1)
            address = node.src.split('?')[0];
        else
            address = node.src;
        node.src = address + "?time=" + new Date().getTime();

        setTimeout(start_refresh, delay);
    })();
}
window.onload = function () {
    var node = document.getElementById('processor');
    refresh(node);
}