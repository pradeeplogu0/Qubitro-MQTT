const express = require('express')
const cors = require('cors')
const axios = require('axios')
require('dotenv').config()

// middlewares
const app = express()
app.use(express.json())
app.use(cors())
app.set("view engine", "ejs")

// routes
app.get('/', (req, res) => {
    let config = {
        method: 'get',
        url: `http://api.qubitro.com/v1/projects/${process.env.PROJECT_ID}/devices/${process.env.DEVICE_ID}/data?keys=LAT,LNG,TEMP,HUMI,PRES,GAS,LIGHT&period=1&limit=1`,
        headers: { 
            'Authorization': `Bearer-${process.env.AUTHORIZATION}`
        }
    }
    axios(config)
    .then(function (response) {
        res.render("map", {data: response.data.response[0]})
    })
    .catch(function (error) {
        console.log(error)
    })
})

// serve
app.listen(process.env.PORT || 1234, () => {
    console.log('Your app run at port 1234')
})