import cls from "@/utils/cls"
import styles from "./ChatInfo.module.css"

import { Button, Typography } from "@/components/UI"
import { ReactSVG } from "react-svg"
import { useChatStore } from "@/state/chat"
import { forwardRef } from "react"

interface ChatInfoProps {
    className?: string
    onClickCloseMobile?: () => void
}

export const ChatInfo = forwardRef<HTMLDivElement, ChatInfoProps>(({
    className = "",
    onClickCloseMobile
}, ref) => {
    const name = useChatStore(state => state.data?.name)

    return (
        <div className={cls(styles.chat_info, className)} ref={ref}>
            <Button className={styles.return_button} variant="icon" onClick={onClickCloseMobile}>
                <ReactSVG src="/close-x.svg" />
            </Button>
            <ReactSVG
                className={styles.icon}
                src="/account.svg"
            />
            <Typography className={styles.name} tag="h2" variant="title-4">
                {name ? name : "Chat"}
            </Typography>
            <Typography className={styles.last_seen} tag="span" variant="text_small">
                last seen
            </Typography>
        </div>
    )
})
