select E.Dept, AVG(E.GradeEarned) as "Average GPA"
from Enrolled  E
GROUP BY E.Dept
ORDER BY E.Dept DESC;
exit
