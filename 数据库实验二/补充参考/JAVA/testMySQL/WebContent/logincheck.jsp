<%@ page language="java" import="java.util.*"  import="java.sql.*" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>欢迎来到登录界面</title>
</head>
<body>
   <%
        Connection conn = null;
        PreparedStatement pstmt = null;
        ResultSet rs = null;
        String driverName = "com.mysql.cj.jdbc.Driver";     
        String userName = "root";
        String userPwd = "MySQL1234";
        String dbName = "company";
        
        String url = "jdbc:mysql://127.0.0.1:3306/company?serverTimezone=GMT%2B8&useSSL=false";
        request.setCharacterEncoding("UTF-8");
        Class.forName(driverName);
        //conn = DriverManager.getConnection(url);
        conn = DriverManager.getConnection(url, userName, userPwd);
        String sql = "select * from user where username=? and password=?"; 
        pstmt = conn.prepareStatement(sql);
        String user = request.getParameter("username");
        String password = request.getParameter("password");
        pstmt.setString(1, user);
        pstmt.setString(2, password);
        rs = pstmt.executeQuery();
        if(rs.next()) {
            %><center><h1>登陆成功！</h1></center>
        <%}
        else {
            %><center><h1>用户名或密码错误！</h1></center>
        <%}
        if(rs != null) {
            rs.close();
        }
        if(pstmt != null) {
            pstmt.close();
        }
        if(conn != null) {
            conn.close();
        }
         
   %>
</body>
</html>