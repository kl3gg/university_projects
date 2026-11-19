document.addEventListener("DOMContentLoaded", () => {
    const kasteKnapp = document.getElementById("Kast");
    const terningC = document.getElementById("terningCon");
    const sumTekst = document.getElementById("sumTotal");
    const feilmelding = document.getElementById("feilmelding");

    kasteKnapp.addEventListener("click", kastTerninger);

    function kastTerninger() {
        const antallTerninger = parseInt(document.getElementById("antallT").value);
        const antallSider = parseInt(document.getElementById("antallS").value);

        terningC.innerHTML = "";
        sumTekst.textContent = "";
        feilmelding.textContent = "";

        // input validation
        if (isNaN(antallTerninger) || antallTerninger < 1) {
            feilmelding.textContent = "Du må kaste minst én terning";
            return;
        }

        if (isNaN(antallSider) || antallSider < 2) {
            feilmelding.textContent = "En terning må minst ha to sider";
            return;
        }
        let sum = 0;

        for (let i = 0; i < antallTerninger; i++) {
            const kast = Math.floor(Math.random() * antallSider) + 1;
            sum += kast;

            const terningElement = document.createElement("div");
            terningElement.className = "terning";
            terningElement.textContent = kast;

            terningC.appendChild(terningElement);
        }

        sumTekst.textContent = `Total sum: ${sum}`;
    }

});