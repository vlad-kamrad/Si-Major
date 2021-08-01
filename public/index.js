const out = document.querySelector("span");
let text = ["🅰️", "🅱️", "🅾️", "🅱️", "🅰️"];

setInterval(() => {
  const _ = text.shift();
  text.push(_);
  out.textContent = text.join(" ");
}, 750);
