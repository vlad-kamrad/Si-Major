const out = document.querySelector("span");
const text = ["😂", "😇", "🥳", "😱", "👻"];

setInterval(() => {
  const _ = text.shift();
  text.push(_);
  out.textContent = text.join(" ");
}, 750);
