import "./ascii-video.css";
import { useEffect, useRef, useState } from "react";
import { createAsciiPlayerJsonl } from "./asciiPlayerJsonl";

export default function AsciiVideoJsonl({
  framesPath,
  preferGzip = true,
}) {
  const preRef = useRef(null);
  const [isReady, setIsReady] = useState(false);

  useEffect(() => {
    if (!preRef.current) return;

    const player = createAsciiPlayerJsonl({
      container: preRef.current,
      framesPath,
      preferGzip,
      onReady: () => {
        setIsReady(true);
      }
    });

    player.start();

    return () => {
      player.stop();
    };
  }, [framesPath, preferGzip]);

  return (
    <pre
      ref={preRef}
      className="ascii-surface"
      style={{ opacity: isReady ? 1 : 0 }}
    />
  );
}
