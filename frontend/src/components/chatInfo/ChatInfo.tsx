import cls from "@/utils/cls"
import styles from "./ChatInfo.module.css"

import { Typography } from "@/components/UI"
import { ReactSVG } from "react-svg"
import { useChatStore } from "@/state/chat"

interface ChatInfoProps {
    className?: string
}

export const ChatInfo = ({
    className = ""
}: ChatInfoProps) => {
    const name = useChatStore(state => state.data?.name)

    return (
        <div className={cls(styles.chat_info, className)}>
            <ReactSVG
                className={styles.icon}
                src="/account.svg"
            />
            <Typography tag="h2" variant="title-4">
                {name ? name : "Chat"}
            </Typography>
            <Typography tag="span" variant="text_small">
                last seen
            </Typography>
        </div>
    )
}
