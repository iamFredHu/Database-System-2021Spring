#encoding:utf-8

import os

DEBUG=True

SECRET_KEY=os.urandom(24)

# 数据库配置
HOSTNAME='127.0.0.1'
PORT='3306'
DATABASE='company'
USERNAME='root'
PASSWORD='MySQL1234'

DB_URI='mysql+mysqlconnector://{}:{}@{}:{}/{}'.format(USERNAME,PASSWORD,HOSTNAME,PORT,DATABASE)

SQLALCHEMY_DATABASE_URI=DB_URI
SQLALCHEMY_TRACK_MODIFICATIONS=False
