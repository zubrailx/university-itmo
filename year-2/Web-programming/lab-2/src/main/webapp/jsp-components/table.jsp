<%@ page import="se.nikit.lab2.model.Point" %>
<%@ page import="java.util.Collections" %>
<%@ page import="java.util.List" %>

<%@ page contentType="text/html;charset=UTF-8" %>

<jsp:useBean id="pointbean" class="se.nikit.lab2.model.PointBean" scope="session"/>

<table class="table">
    <thead class="table-head table__head">
    <tr class="table-head__row">
        <th class="table-head__cell">Coordinate X</th>
        <th class="table-head__cell">Coordinate Y</th>
        <th class="table-head__cell">Radius</th>
        <th class="table-head__cell">Fact of hit</th>
        <th class="table-head__cell">Submit time</th>
        <th class="table-head__cell">Execution time</th>
    </tr>
    </thead>
    <tbody class="table-body table__body">
    <%
        List<Point> pointList = pointbean.getList();

        for (Point p : pointList) {
    %>
    <tr class="table-body__row">
        <td class="table-body__cell"><%=String.format("%.4f", p.getX())%>
        </td>
        <td class="table-body__cell"><%=String.format("%.4f", p.getY())%>
        </td>
        <td class="table-body__cell"><%=String.format("%.4f", p.getR())%>
        </td>
        <td class="table-body__cell"><%=p.isHit()%>
        </td>
        <td class="table-body__cell"><%=p.getLocalTimeFormatted()%>
        </td>
        <td class="table-body__cell"><%=String.format("%.4f", p.getExecutionTimeSeconds() * Math.pow(10, 3))%> ms</td>
    </tr>
    <%
        }
    %>
    </tbody>
</table>
