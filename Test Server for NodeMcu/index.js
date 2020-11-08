const express = require('express')
const { Client } = require('pg')
const dotenv = require('dotenv');

// DOTENV
dotenv.config();

// EXPRESS
const app = express()
app.use(express.json());
const port = 80

// POSTGRES
const client = new Client({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_NAME,
})
client.connect()
client.query('SELECT $1::text as message', ['Database Connected'], (err, res) => {
  console.log(err ? err.stack : res.rows[0].message)
})  


app.get('/', (req, res) => {
    res.send('Hello World!')
})

app.post('/nodemcutest', async (req, res) => {
    console.log('> POST', req.body)
    const result = await client.query('INSERT INTO nodemcutest (time) VALUES (now())', [])
    res.send({
        ok: true,
        result: result
    })
})

app.listen(port, () => {
  console.log(`Server Listening at http://localhost:${port}`)
})