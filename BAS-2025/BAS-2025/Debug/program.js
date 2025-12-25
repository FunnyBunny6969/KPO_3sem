function random(min, max) {return Math.floor(Math.random() * (max - min + 1)) + min;}
function pow(a, b) {return Math.pow(a, b);}

function demonstrate_underflow( ) {
console.log("демонстрации переполнения");
let a = 0;
let b = 0;
let c = 0;
a = (5) >>> 0;
console.log("a = ");
console.log((a) >>> 0);
b = (10) >>> 0;
console.log("b = ");
console.log((b) >>> 0);
c = (a - b) >>> 0;
console.log("c = ");
console.log((c) >>> 0);
return (0) >>> 0;
};
function use_standard_library( ) {
console.log("Вызов библиотечных функций");
console.log("pow(uint, uint)");
console.log((pow(2, 2)) >>> 0);
console.log((pow(3, 3)) >>> 0);
console.log("random(uint min, uint max)");
console.log((random(0, 100)) >>> 0);
console.log((random(1, 9)) >>> 0);
return (0) >>> 0;
};
function bitwise_ops( result, op_type) {
switch ((op_type) >>> 0) {
case 0:
result = ((result & 15)) >>> 0;
break;
case 1:
result = ((result | 240)) >>> 0;
break;
case 2:
result = (!(result)) >>> 0;
break;
default: 
result = (result * 2) >>> 0;
break;
};
return (result) >>> 0;
};
function main() {
let test_uint = 0;
let test_string = 'S';
let test_char = '$';
console.log("ДО");
console.log("test_uint = ");
console.log((test_uint) >>> 0);
console.log("test_string = ");
console.log(test_string);
console.log("test_char = ");
console.log(test_char);
test_uint = (5) >>> 0;
test_string = "ТЕСТОВАЯ СТРОКА";
test_char = '@';
console.log("ПОСЛЕ");
console.log("test_uint = ");
console.log((test_uint) >>> 0);
console.log("test_string = ");
console.log(test_string);
console.log("test_char = ");
console.log(test_char);
(demonstrate_underflow()) >>> 0;
(use_standard_library()) >>> 0;
console.log("=== ТЕСТ БИТОВЫХ ОПЕРАЦИЙ ===");
console.log("bitwise_ops(0x12, 0x00) = ");
console.log((bitwise_ops(18, 0)) >>> 0);
console.log("bitwise_ops(0x12, 0x01) = ");
console.log((bitwise_ops(18, 1)) >>> 0);
console.log("bitwise_ops(0x12, 0x02) = ");
console.log((bitwise_ops(18, 2)) >>> 0);
console.log("bitwise_ops(0x12, 0x03) = ");
console.log((bitwise_ops(18, 3)) >>> 0);
return (0) >>> 0;
};
main();