const routers = [
    {
        path: '/',
        meta: {
            title: 'Client'
        },
        component: (resolve) => require(['./views/index.vue'], resolve)
    },
    {
        path: '/main',
        meta: {
            title: 'Main Control'
        },
        component: (resolve) => require(['./views/main.vue'], resolve)
    }
];
export default routers;