export default async function handler(req, res) {
  try {
    const token = process.env.DISCOGS_TOKEN;
    if (!token) return res.status(500).json({ error: "Hiányzó DISCOGS_TOKEN a Vercel env-ben" });

    const endpoint = req.query.endpoint;
    if (!endpoint) return res.status(400).json({ error: "Hiányzó endpoint paraméter" });

    // Biztonsági minimál-szűrés (ne lehessen tetszőleges domain)
    if (endpoint.includes("http://") || endpoint.includes("https://")) {
      return res.status(400).json({ error: "Érvénytelen endpoint" });
    }

    const url = "https://api.discogs.com/" + endpoint;

    const r = await fetch(url, {
      headers: {
        "Accept": "application/vnd.discogs.v2+json",
        "Authorization": "Discogs token=" + token,
        // Discogs gyakran elvár User-Agent-et:
        "User-Agent": "LPKatalogus/1.0 (+vercel)"
      }
    });

    const text = await r.text();
    res.status(r.status).setHeader("Content-Type", r.headers.get("content-type") || "application/json");
    return res.send(text);
  } catch (e) {
    return res.status(500).json({ error: "Szerver hiba", detail: String(e?.message || e) });
  }
}
