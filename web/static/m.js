const routes = [
  { path: '/snake', component: Snake },
  { path: '/cube', component: Cube }
]
const router = new VueRouter({
  routes
})
new Vue({
    router
}).$mount(`#app`);

var socket = io.connect('http://' + document.domain + ':' + location.port);
socket.on('connect', function() {
    socket.emit('client_connected', {data: 'Connected'});
});

socket.on('clientpush', (data) => {
    router.push(data);
});
