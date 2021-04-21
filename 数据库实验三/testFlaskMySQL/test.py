from flask import Flask,render_template
from flask import request
import mysql.connector

conn=mysql.connector.connect(user="root",password="MySQL1234",database="testdb")#数据库连接
cursor=conn.cursor()

app=Flask(__name__)#引入Flask

@app.route('/',methods=['GET'])#跳转至login.html，请求方式GET
def show():
    return render_template('login.html')

@app.route('/login',methods=['POST', 'GET'])
def login():
    if request.method == 'GET':
        return render_template('login.html')
    else:
        username = request.form['username'] #界面传值
        password = request.form['password'] #界面传值
        if(len(username) == 0 | len(password) == 0):
            return render_template('login.html')

        cursor.execute('select username from test')#查询test表查询用户名
        usernames=cursor.fetchall()

        for user in usernames:
            if request.form['username']==user[0]:
                cursor.execute('select password from test where username=%s',(user[0],))

                pw=cursor.fetchall()#从test表中获取密码

                if request.form['password']==pw[0][0]:#如果页面输入的password匹配test表返回的密码
                    return '<h>欢迎回来,%s！</h>'%user[0]

                return '<h>账号、密码错误！</h>'

        cursor.close()#关闭游标
        conn.close()#关闭连接

@app.route('/regist',methods=['POST', 'GET'])#表单提交
def regist():
    if request.method == 'GET':
        return render_template('regist.html')
    else:
        user = request.form.get('user')
        pw = request.form.get('pw')

        cursor.execute('insert into test(username, password) values (%s, %s)', (user, pw))#把注册信息加入test表

        return '<h>注册成功！请登录。</h><form action="/login" method="get"><p><button type="submit">返回登录</button></p></form>'

        cursor.close()#关闭游标
        conn.close()#关闭连接

if __name__=='__main__':
    app.run()
