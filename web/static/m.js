
const routes = [
  { path: '/snake', component: Snake },
  { path: '/cube', component: Cube },
  { path: '/pong', component: Pong }
]
const router = new VueRouter({
  routes,
  hashbang: false
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
