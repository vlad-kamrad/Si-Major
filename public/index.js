const out = document.querySelector("span");
const text = ["😂", "😇", "🥳", "😱", "👻"];

setInterval(() => {
  const _ = text.pop();
  text.unshift(_);
  out.textContent = text.join(" ");
}, 3000);
