<%@ page contentType="text/html; charset=UTF-8" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <!--Css-->
    <link rel="stylesheet" type="text/css" href="css/app.css?v=21"/>
    <!--Fonts-->
    <link href="https://fonts.googleapis.com/css2?family=Lato:wght@300;400;700&display=swap" rel="stylesheet">
    <!--Libraries-->
    <script src="js/jquery.js"></script>
</head>
<body>
    <header class="page-header">
        <div class="page-header__wrapper">
            <div class="page-header__personal">
                <span class="page-header__item">Nikita Kulakov Vasilevich</span>
                <span class="page-header__item">P3230</span>
            </div>
            <a class="page-header__link" target="_blank" href="https://github.com/zubrailx/web-lab-2">Github repository</a>
        </div>
    </header>
    <main class="page-content">
        <header class="task-header">
            <div class="task-header__wrapper">
                <span class="task-header__item task__name">Лабораторная работа №2. Вариант 30109.</span>
            </div>
        </header>
        <section class="section-lab">
            <!--||graph-->
            <jsp:include page="jsp-components/graph.jsp"/>
            <!--||form-->
            <jsp:include page="jsp-components/form.jsp"/>
        </section>

        <!--||table-->
        <jsp:include page="jsp-components/table.jsp"/>
    </main>
    <footer class="page-footer">
        <div class="page-footer__wrapper">
            <span class="page-footer__note">Web-programming 2021</span>
        </div>
    </footer>
    <script src="js/controller.js?v=24"></script>
    <script src="js/view.js?v=4"></script>
</body>
</html>