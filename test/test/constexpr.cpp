template<typename Itr>
bool is_sorted
(
Itr begin
,
const
 Itr 
&
end
)
{
Itr 
start 
=
 begin
;
++
begin
;
while
(
begin 
!=
 end
)
{
if
(!(*
start 
<
*
begin
))
{
return
false
;
}
start 
=
 begin
;
++
begin
;
}
return
true
;
}
template
<
typename
 T
>
bool
is_sorted
(
const
 std
::
initializer_list
<
T
>
&
l
)
{
return
is_sorted
(
l
.
begin
(),
 l
.
end
());
}
int
main
()
{
return
 is_sorted
({
1
,
2
,
3
,
4
,
5
});
}
