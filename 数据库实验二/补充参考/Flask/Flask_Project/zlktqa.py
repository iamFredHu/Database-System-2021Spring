#encoding:utf-8

from flask import Flask,render_template,request,redirect,url_for,session
import config
from models import User,Question,Answer
from exts import db
from decorators import login_required
from datetime import datetime

app=Flask(__name__)
app.config.from_object(config)
db.init_app(app)


# 首页函数
@app.route('/')
def index():
    context={
        'questions':Question.query.all()
    }
    return render_template('index.html',**context)

# 登录函数
@app.route('/login/',methods=['GET','POST'])
def login():
    if request.method=='GET':
        return render_template('login.html')
    else:
        telephone = request.form.get('telephone')
        password = request.form.get('password')
        # 在数据库中查找手机号码和密码是否对应
        user=User.query.filter(User.telephone==telephone,User.password==password).first()
        # 如果正确
        if user:
            session['user_id']=user.id
            # 如果想在31天内都不需要登录
            session.permanent=True
            # 登录成功跳转到首页
            return redirect(url_for('index'))
        else:
            return u'手机号码或者密码错误，请确认后再登录！'


# 注册函数
@app.route('/regist/',methods=['GET','POST'])
def regist():
    if request.method=='GET':
        return render_template('regist.html')
    else:
        telephone=request.form.get('telephone')
        username = request.form.get('username')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')

        # 手机号码验证 如果被注册了 就不能够再次注册
        user=User.query.filter(User.telephone==telephone).first()
        if user:
            return u'该手机号码已经被注册，请更换手机号码！'
        else:
            # password1和password2相等才可以
            if password1 != password2:
                return u'两次密码不相等，请核对后再次填写！'
            else:
                #可以进行注册
                user=User(telephone=telephone,username=username,password=password1)
                # 添加到数据库中
                db.session.add(user)
                # 进行事务提交
                db.session.commit()
                # 如果注册成功，页面跳转到登录界面
                return redirect(url_for('login'))


# 注销函数
@app.route('/logout/')
def logout():
    # 删除cookie
    # session.pop('user_id')
    del session['user_id']
    # 删除所有cookie
    # session.clear()
    return redirect(url_for('login'))


# 发布问答函数
@app.route('/question/',methods=['GET','POST'])
@login_required
def question():
    if request.method=='GET':
        return render_template('question.html')
    else:
        title=request.form.get('title')
        content=request.form.get('content')
        question=Question(title=title,content=content)
        user_id=session.get('user_id')
        user=User.query.filter(User.id==user_id).first()
        question.author=user
        db.session.add(question)
        db.session.commit()
        return redirect(url_for('index'))


# 上下文函数
@app.context_processor
def my_context_processor():
    # 查找cookie
    user_id=session.get('user_id')
    if user_id:
        user=User.query.filter(User.id==user_id).first()
        if user:
            return {'user':user}
    return {}


# 问答详情函数
# question_id为问题的id
@app.route('/detail/<question_id>/')
def detail(question_id):
    question_model=Question.query.filter(Question.id==question_id).first()
    return render_template('detail.html',question=question_model)

# 添加评论函数
@app.route('/add_answer/',methods=['POST'])
@login_required
def add_answer():
    content=request.form.get('answer_content')
    question_id=request.form.get('question_id')
    answer=Answer(content=content)
    user_id=session['user_id']
    user=User.query.filter(User.id==user_id).first()
    answer.author=user
    question=Question.query.filter(Question.id==question_id).first()
    answer.question=question
    # create_time=datetime.now()
    # answer.create_time=create_time
    db.session.add(answer)
    db.session.commit()
    return redirect(url_for('detail',question_id=question_id))


if __name__=='__main__':
    app.run()


