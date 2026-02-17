const CACHE_NAME = "lp-katalogus-v1";
const APP_SHELL = [
  "./",
  "./index.html",
  "./manifest.json"
  // Add icon paths if you provide them:
  // "./icons/icon-192.png",
  // "./icons/icon-512.png"
];

self.addEventListener("install", (event) => {
  event.waitUntil(
    caches.open(CACHE_NAME).then((cache) => cache.addAll(APP_SHELL))
  );
  self.skipWaiting();
});

self.addEventListener("activate", (event) => {
  event.waitUntil((async () => {
    const keys = await caches.keys();
    await Promise.all(keys.map((k) => (k === CACHE_NAME ? null : caches.delete(k))));
    await self.clients.claim();
  })());
});

// Cache-first for app shell, network-first for everything else
self.addEventListener("fetch", (event) => {
  const req = event.request;
  const url = new URL(req.url);

  // Only handle same-origin
  if (url.origin !== location.origin) return;

  event.respondWith((async () => {
    const cached = await caches.match(req);
    if (cached) return cached;

    try{
      const fresh = await fetch(req);
      return fresh;
    }catch{
      // fallback to index for SPA navigation (optional)
      const fallback = await caches.match("./index.html");
      return fallback || new Response("Offline", { status: 503, statusText: "Offline" });
    }
  })());
});
